// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicWorldSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

#include "HAL/IConsoleManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Commandlets/GatherTextFromSourceCommandlet.h"

#include "Systems/SystemicCore.h"
#include "Systems/SystemicGameplayTags.h"

#include "Systems/Events/SystemicEvent.h"
#include "Systems/Events/EventData/SystemicContactEventData.h"
#include "Systems/Events/EventData/SystemicHealthEventData.h"
#include "Systems/Events/EventData/SystemicInteractionEventData.h"
#include "Systems/Events/EventData/SystemicTemperatureEventData.h"
#include "Systems/Events/EventData/SystemicTraitChangedEventData.h"

#include "Systems/Conditions/SystemicCondition.h"
#include "Systems/Reactions/SystemicReaction.h"
#include "Systems/Rules/SystemicRule.h"
#include "Systems/Rules/SystemicRuleContext.h"

// Setup console commands and variables.
namespace JoyCore::Systems
{
	static TAutoConsoleVariable<int32> CVarSystemProcessEventsImmediately(
		TEXT("JoyCore.Systems.ProcessEventsImmediately"),
		0,
		TEXT("Controls when events are processed; 0 = queue and dispatch during Tick (Default), 1 = process immediately in EmitEvent."),
		ECVF_Default);

	static TAutoConsoleVariable<int32> CVarSystemDebugSystemicEvents(
		TEXT("JoyCore.Systems.DebugSystemicEvents"),
		0,
		TEXT("Enables additional debug logging while processing Systemic Events (Verbosity: Verbose); 0 = Disabled, 1 = Enabled."),
		ECVF_Default);
}

// Method to fill out the EventDataStructureMap with FSystemicEvent-based payload types for each event tag.
void USystemicWorldSubsystem::InitializeEventDataStructureMap()
{
	// Initialize the EventDataStructureMap with event mappings (as high-level as possible).
	//	NOTE (trent, 5/25/26): I don't love this approach, but I prefer event data as a struct instead of a UObject and this is trying to make a lot of BP accommodations.
	EventDataStructureMap.Empty();
	
	// Object instance lifecycle event mappings (generic event).
	AddEventStructMappings({
		TAG_System_Event_Created,
		TAG_System_Event_Destroyed
		}, FSystemicEventData::StaticStruct());

	// Trait change event mappings.
	AddEventStructMappings({
		TAG_System_Event_TraitsChanged,
		TAG_System_Event_StateChanged,

		TAG_System_Event_Broken,
		TAG_System_Event_Electrified,
		TAG_System_Event_Frozen,
		TAG_System_Event_Ignited,
		TAG_System_Event_Wet,
		
		TAG_System_Event_LifecycleChanged,
		TAG_System_Event_Lifecycle_Spawned,
		TAG_System_Event_Lifecycle_Downed,
		TAG_System_Event_Lifecycle_Revived,
		TAG_System_Event_Lifecycle_Killed,
		
		TAG_System_Event_World_TimeOfDayChanged,
		TAG_System_Event_World_WeatherChanged
		}, FSystemicTraitChangedEventData::StaticStruct());
	
	// Health event mappings.
	AddEventStructMappings({
		TAG_System_Event_HealthChanged,
		TAG_System_Event_HealthMaxChanged
		}, FSystemicHealthEventData::StaticStruct());

	// Interaction event map.
	AddEventStructMappings({
		TAG_System_Event_Interacted
		}, FSystemicInteractionEventData::StaticStruct());

	// Temperature change event map.
	AddEventStructMappings({
		TAG_System_Event_TemperatureChanged
		}, FSystemicTemperatureEventData::StaticStruct());

	// Contact event mappings.
	AddEventStructMappings({
		TAG_System_Event_Contact_Hit,
		TAG_System_Event_Contact_OverlapBegin,
		TAG_System_Event_Contact_OverlapEnd
		}, FSystemicContactEventData::StaticStruct());
}

// Get the event data struct for a given event tag.
const UScriptStruct* USystemicWorldSubsystem::GetEventDataStructForEvent(const FGameplayTag& EventTag) const
{
	if(!EventDataStructureMap.Contains(EventTag))
	{
		ensure(EventDataStructureMap.Contains(EventTag));
		UE_LOG(LogJoyCoreSystems, Error, TEXT("No data structure mapped for SystemicEvent: %s"), *EventTag.ToString());
		return nullptr;
	}
	
	return EventDataStructureMap[EventTag];
}

// Process a systemic event by running it through all valid rules and conditions to see if it's valid.
bool USystemicWorldSubsystem::ProcessSystemicEvent(const FSystemicEvent& Event, const bool bIgnoreRuleCooldowns)
{
	const bool bDebugSystemicEvents = JoyCore::Systems::CVarSystemDebugSystemicEvents.GetValueOnGameThread() == 1;
	UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("Processing SystemicEvent: %s"), *Event.EventTag.ToString());

	// Get all matching rules.
	TArray<FSystemicRuleRuntimeData*> matchingRules = FindMatchingRules(Event.EventTag);
	if(matchingRules.IsEmpty())
	{
		// There isn't a strict requirement to have a rule for every event but it's useful to know when it happens (just demoted to a Verbose log).
		UE_LOG(LogJoyCoreSystems, Verbose, TEXT("No Rules defined to process SystemicEvent: %s"), *Event.EventTag.ToString());
		return false;
	}

	// Start building up the trace data.
	FSystemicTrace trace;
	trace.EventTag = Event.EventTag;

	// Run through all matching rules for this event (all are sorted by priority in the map).
	for(FSystemicRuleRuntimeData* pRuleData : matchingRules)
	{
		USystemicRule* pRule = pRuleData->Rule.Get();
		const bool ruleIsValid = IsValid(pRule);
		ensure(ruleIsValid);
		
		if(!ruleIsValid)
		{
			UE_LOG(LogJoyCoreSystems, Error, TEXT("Invalid rule encountered while processing SystemicEvent: %s"), *Event.EventTag.ToString());
			continue;
		}
		
		// If a rule is on cooldown then this event cannot be processed
		if(!bIgnoreRuleCooldowns && (pRuleData->Cooldown > 0.0f))
		{
			trace.RuleNameAndResultList.Add(TPair<FName, bool>(pRule->GetRuleName(), false));

			UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicRule (%s) is on cooldown and cannot process event (%s)."), *pRule->GetRuleName().ToString(), *Event.EventTag.ToString());
			continue;
		}

		// Execute the rule and evaluate its conditions.
		FSystemicRuleContext ruleContext;
		if(!EvaluateRule(pRule, Event, trace, ruleContext))
		{
			trace.RuleNameAndResultList.Add(TPair<FName, bool>(pRule->GetRuleName(), false));

			UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicEvent (%s) failed to process rule %s."), *Event.EventTag.ToString(), *pRule->GetRuleName().ToString());
			continue;
		}

		// Update the trace log.
		trace.RuleNameAndResultList.Add(TPair<FName, bool>(pRule->GetRuleName(), true));

		// Execute the reactions this rule has specified.
		bool bAllReactionsSucceeded = true;
		if(!ExecuteReactions(pRule, Event, ruleContext, trace))
		{
			bAllReactionsSucceeded = false;

			UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicEvent (%s) failed to process all reactions for rule %s."), *Event.EventTag.ToString(), *pRule->GetRuleName().ToString());
		}
		
		// Put this rule on cooldown.
		pRuleData->Cooldown = pRule->GetCooldown();
		if(pRuleData->Cooldown > 0.0f)
		{
			// Need to reduce the cooldown on tick.
			RulesOnCooldown.AddUnique(pRuleData);
		}

		//Print successful handling message.
		UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("SystemicEvent (%s) successfully processed by rule %s (Cooldown is %f)."), *Event.EventTag.ToString(), *pRule->GetRuleName().ToString(), pRuleData->Cooldown);

		if(bDebugSystemicEvents)
		{
			// Print the evaluation log if event debugging is enabled.
			UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicEvent (%s) evaluation log:\n%s"), *Event.EventTag.ToString(), *trace.GetEvaluationLogAsString());
		}

		return bAllReactionsSucceeded;
	}
	
	UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicEvent (%s) was unhandled."), *Event.EventTag.ToString());

	if(bDebugSystemicEvents)
	{
		// Print the evaluation log if event debugging is enabled.
		UE_LOG(LogJoyCoreSystems, Log, TEXT("SystemicEvent (%s) evaluation log:\n%s"), *Event.EventTag.ToString(), *trace.GetEvaluationLogAsString());
	}

	return false;
}

// Execute the passed-in rule triggered by the referenced event.
bool USystemicWorldSubsystem::EvaluateRule(USystemicRule* Rule, const FSystemicEvent& Event, FSystemicTrace& Trace, FSystemicRuleContext& RuleContextOut) const
{
	UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("Executing SystemicRule: %s"), *Rule->GetRuleName().ToString());
	
	// Fill out the rule context.
	RuleContextOut.Instigator = Event.Instigator;
	RuleContextOut.Target = Event.Target;
	RuleContextOut.Source = Event.Source;
	
	for(const USystemicCondition* pCondition : Rule->GetConditionList())
	{
		if(!IsValid(pCondition))
		{
			// Invalid condition; skip evaluation.
			UE_LOG(LogJoyCoreSystems, Warning, TEXT("SystemicRule %s failed evaluation due to having an invalid condition (during ::EvaluateRule)."), *Rule->GetRuleName().ToString());
			return false;
		}
		
		if(!pCondition->Evaluate(Event, RuleContextOut, Trace))
		{
			// Condition failed; stop evaluating this rule.
			UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("SystemicRule %s failed condition %s."), *Rule->GetRuleName().ToString(), *pCondition->GetName());
			return false;
		}
	}
	
	return true;
}

// Execute the passed-in rule triggered by the referenced event.
bool USystemicWorldSubsystem::ExecuteReactions(USystemicRule* Rule, const FSystemicEvent& Event, FSystemicRuleContext& RuleContext, FSystemicTrace& Trace) const
{
	UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("Executing the Reactions in SystemicRule: %s"), *Rule->GetRuleName().ToString());
	
	bool bReactionSuccess = true;
	for(USystemicReaction* pReaction : Rule->GetReactionList())
	{
		if(!IsValid(pReaction))
		{
			// Invalid reaction; skip execution.
			bReactionSuccess = false;

			UE_LOG(LogJoyCoreSystems, Warning, TEXT("SystemicRule %s has an invalid reaction (during ::ExecuteReactions)."), *Rule->GetRuleName().ToString());
			continue;
		}
		
		if(!pReaction->Execute(Event, RuleContext, Trace))
		{
			// Reaction failed; mark the overall method as failed but continue executing remaining reactions.
			bReactionSuccess = false;

			UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("SystemicRule %s failed to execute reaction %s."), *Rule->GetRuleName().ToString(), *pReaction->GetName());
		}
	}
	
	return bReactionSuccess;
}

// Find all rules matching the passed-in event tag.
TArray<FSystemicRuleRuntimeData*> USystemicWorldSubsystem::FindMatchingRules(const FGameplayTag& EventTag)
{
	TArray<FSystemicRuleRuntimeData*> matchingRules;
	
	// Loop through the entire rule map and find all matching rules that adhere to the passed-in flags.
	if(FSystemicMappedRules* pMappedRules = RuleMap.Find(EventTag))
	{
		for(FSystemicRuleRuntimeData& ruleData : pMappedRules->Rules)
		{
			if(USystemicRule* pRule = ruleData.Rule.Get())
			{
				if(!pRule->GetIsEnabled())
				{
					// Rule isn't enabled; skip.
					continue;
				}

				// Found a matching rule.
				matchingRules.Add(&ruleData);
			}
		}
	}

	return matchingRules;
}

// Get the USystemicWorldSubsystem instance for the given world context object.
USystemicWorldSubsystem* USystemicWorldSubsystem::Get(const UObject* WorldContextObject)
{
	if(!IsValid(WorldContextObject))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicWorldSubSystem::Get failed due to an invalid context object."));
		return nullptr;		
	}
	
	const UWorld* pWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	USystemicWorldSubsystem* pSystemicWorldSubsystem = pWorld->GetSubsystem<USystemicWorldSubsystem>();
	
	bool result = IsValid(pWorld) && IsValid(pSystemicWorldSubsystem);
	check(result);
	return pSystemicWorldSubsystem;
}

// Blueprint-exposed method to update the Event-to-EventData map.
bool USystemicWorldSubsystem::AddEventStructMapping(const FGameplayTag& EventTag, UPARAM(meta=(AllowedClasses="SystemicEventData")) const UScriptStruct* EventDataStruct, bool bAllowOverwrite)
{
	return(AddEventStructMappings({ EventTag }, EventDataStruct, bAllowOverwrite));
}

// Blueprint-exposed method to update the Event-to-EventData map.
bool USystemicWorldSubsystem::AddEventStructMappings(const TArray<FGameplayTag>& EventTags, const UScriptStruct* EventDataStruct, bool bAllowOverwrite)
{
	check(IsValid(EventDataStruct));
	
	bool bResult = true;
	for(const FGameplayTag& eventTag : EventTags)
	{
		if(!bAllowOverwrite && EventDataStructureMap.Contains(eventTag))
		{
			// Not allowed to overwrite an event mapping.
			UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicWorldSubSystem::AddEventStructMappings failed due to an existing mapping for Event Tag: %s (bAllowOverrides was false)."), *eventTag.ToString());

			bResult = false;
			continue;
		}
		
		// Add the mapping.
		EventDataStructureMap.Add(eventTag, EventDataStruct);
	}
	
	// Will be false if even one of the mappings fails.
	return bResult;
}

// Make a systemic event with the proper payload type.
bool USystemicWorldSubsystem::MakeSystemicEvent(FSystemicEvent& EventOut, const FGameplayTag& EventTag, const FGameplayTag& Priority, const ESystemicEventSubject Subject, UObject* Source, AActor* Instigator, UObject* Target)
{
	EventOut.EventTag = EventTag;
	EventOut.Priority = Priority;
	EventOut.Subject = Subject;
	EventOut.Source = Source;
	EventOut.Instigator = Instigator;
	EventOut.Target = Target;

	EventOut.EventDataInstance.InitializeAs(GetEventDataStructForEvent(EventTag));
	return true;
}

// Emit (dispatch) an event for processing either immediately or in the ::Tick queue.
bool USystemicWorldSubsystem::EmitEvent(const FSystemicEvent& Event)
{
	if(JoyCore::Systems::CVarSystemProcessEventsImmediately.GetValueOnGameThread() == 0)	
	{
		// Queue up the event for processing next ::Tick.
		EventQueue.Add(Event);
	}
	else
	{
		// Process the event immediately.
		ProcessSystemicEvent(Event);
	}
	
	return true;
}

// Static call to emit an event (for blueprint exposure).
bool USystemicWorldSubsystem::EmitEvent(const UObject* WorldContextObj, const FSystemicEvent& Event)
{
	if(!IsValid(WorldContextObj))
	{
		// Invalid world context, can't grab the subsystem.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicWorldSubSystem::EmitEvent has a null world context (Event Tag: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	const UWorld* pWorld = WorldContextObj->GetWorld();
	if(!IsValid(pWorld))
	{
		// Invalid world, can't grab the subsystem.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicWorldSubSystem::EmitEvent has a null world context (Event Tag: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	USystemicWorldSubsystem* pSubsystem = pWorld->GetSubsystem<USystemicWorldSubsystem>();
	ensure(IsValid(pSubsystem));
	if(!IsValid(pSubsystem))
	{
		// Subsystem not found.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("No valid SystemicWorldSubSystem instance (Event Tag: %s)."), *Event.EventTag.ToString());
		return false;
	}

	// Now emit the event.
	return(pSubsystem->EmitEvent(Event));
}

// Register a rule with the subsystem as an Active Rule as well as caching it off in the rule map.
bool USystemicWorldSubsystem::RegisterRule(USystemicRule* RuleIn)
{
	if(!IsValid(RuleIn))
	{
		// Invalid rule; do not register.
		UE_LOG(LogJoyCoreSystems, Warning, TEXT("USystemicWorldSubSystem::RegisterRule has a null rule input."));
		return false;
	}

	for(const FGameplayTag& EventTag : RuleIn->GetTriggerEventTags())
	{
		// Find or add this rule to the rule map cache for easy access by tag.
		FSystemicMappedRules& ruleAssets = RuleMap.FindOrAdd(EventTag);
		
		ruleAssets.EventTag = EventTag;
		ruleAssets.Rules.Add(FSystemicRuleRuntimeData(RuleIn, -1.0f));
		
		// Sort the rule assets by priority.
		ruleAssets.Rules.Sort([](const FSystemicRuleRuntimeData& RuleA, const FSystemicRuleRuntimeData& RuleB)
		{
			return(USystemicCore::GetHigherPriorityRule(RuleA.Rule.Get(), RuleB.Rule.Get()) == RuleA.Rule.Get());
		});

		UE_LOG(LogJoyCoreSystems, Log, TEXT("SystemicRuleAsset added to USystemicWorldSubsystem: %s\nFull Name: %s (Event Tag: %s)."), *RuleIn->GetRuleName().ToString(), *RuleIn->GetFullName(), *EventTag.ToString());
	}
	
	return true;
}

// Return that this subsystem always ticks.
ETickableTickType USystemicWorldSubsystem::GetTickableTickType() const
{
	// Always tick this subsystem.
	return ETickableTickType::Always;
}

// Process events in the queue.
void USystemicWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	{
		int32 idx = 0;

		// Tick down the rules on cooldown.
		while(!RulesOnCooldown.IsEmpty() && idx < RulesOnCooldown.Num())
		{
			FSystemicRuleRuntimeData* pRuleData = RulesOnCooldown[idx];
			pRuleData->Cooldown-= DeltaTime;
			if(pRuleData->Cooldown <= 0.0f)
			{
				// Cooldown is up; remove from cooldown list.
				RulesOnCooldown.Remove(pRuleData);
				continue;
			}

			++idx;
		}
	}

	// Events are processed in a queue if JoyCore.Systems.ProcessEventsImmediately is 0.
	if(JoyCore::Systems::CVarSystemProcessEventsImmediately.GetValueOnGameThread() == 0)
	{
		int32 eventProcessCount = EventProcessCountPerTick;
		
		while(!EventQueue.IsEmpty() && ((eventProcessCount > 0) || (EventProcessCountPerTick == 0)))
		{
			// Run through the queue of events so long as the counter is above zero or EventProcessCountPerTick equals 0 (which means process all events).
			ProcessSystemicEvent(EventQueue[0]);
			EventQueue.RemoveAt(0);

			--eventProcessCount;
		}
	}
}

// Initialize the subsystem by scanning for all rule assets.
void USystemicWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load all Rule Assets defined in this plugin's content folder.
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FTopLevelAssetPath ruleAssetPath = USystemicRule::StaticClass()->GetClassPathName();
	
	// Add the base content folder and plugin's content folder to the scan paths.
	TArray<FString> PathsToScan;
	PathsToScan.Add(TEXT("/Game"));
	PathsToScan.Add(TEXT("/JoyCore"));
	assetRegistryModule.Get().ScanPathsSynchronous(PathsToScan, true);
	
	// Get all Rule Assets by class.
	TArray<FAssetData> assetData;
	assetRegistryModule.Get().GetAssetsByClass(ruleAssetPath, assetData, true);
	
	// Go through and register all the rules with the map and the active rule list if the rule is enabled.
	for(const FAssetData& asset : assetData)
	{
		TObjectPtr<USystemicRule> pRule = Cast<USystemicRule>(asset.GetAsset());
		if(IsValid(pRule))
		{
			// Register the rule.
			RegisterRule(pRule);
		}
		else
		{
			// Log a warning that the asset isn't getting processed properly.
			UE_LOG(LogJoyCoreSystems, Warning, TEXT("Invalid SystemicRuleAsset found: %s"), *asset.GetFullName());
		}
	}
	
	InitializeEventDataStructureMap();
}

// Clear out all the data.
void USystemicWorldSubsystem::Deinitialize()
{
	// Clear the active rule list.
	RulesOnCooldown.Empty();
	EventQueue.Empty();
	RuleMap.Empty();
	
	Super::Deinitialize();
}

// Required function override for profiling.
TStatId USystemicWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USystemicWorldSubsystem, STATGROUP_Tickables);
}