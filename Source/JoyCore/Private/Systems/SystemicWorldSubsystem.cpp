// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicWorldSubsystem.h"

#include "Engine/World.h"

#include "HAL/IConsoleManager.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "Systems/Events/SystemicEvent.h"
#include "Systems/Rules/SystemicRuleAsset.h"

// Define the log category for the JoyCore systems logic.
DEFINE_LOG_CATEGORY(LogJoyCoreSystems);

// Setup console commands and variables.
namespace JoyCore::Systems
{
	static TAutoConsoleVariable<int32> CVarSystemProcessEventsImmediately(
		TEXT("JoyCore.Systems.ProcessEventsImmediate"),
		0,
		TEXT("Controls when events are processed; 0 = queue and dispatch during Tick (Default), 1 = process immediately in EmitEvent."),
		ECVF_Default);
}

// Process a rule triggered by the referenced event.
void USystemicWorldSubsystem::ProcessSystemicEvent(const FSystemicEvent& Event)
{
	UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("Processing SystemicEvent: %s"), *Event.EventTag.ToString());
	
	//	
}

// Execute the passed-in rule triggered by the referenced event.
void USystemicWorldSubsystem::ExecuteRule(USystemicRuleAsset* Rule, const FSystemicEvent& Event)
{
	UE_LOG(LogJoyCoreSystems, VeryVerbose, TEXT("Executing SystemicRule: %s"), *Rule->GetRuleName().ToString());

	//
}

// Find all rules matching the passed-in event tag.
TArray<USystemicRuleAsset*> USystemicWorldSubsystem::FindMatchingRules(FGameplayTag EventTag) const
{
	TArray<USystemicRuleAsset*> matchingRules;
	if(const FSystemicMappedRuleAssets* pMappedRules = RuleMap.Find(EventTag))
	{
		for(const TSoftObjectPtr<USystemicRuleAsset>& ruleAsset : pMappedRules->RuleAssets)
		{
			if(USystemicRuleAsset* pRule = ruleAsset.Get())
			{
				// Found a matching rule.
				matchingRules.Add(pRule);
			}
		}
	}

	return matchingRules;
}

// Emit (dispatch) an event for processing either immediately or in the ::Tick queue.
void USystemicWorldSubsystem::EmitEvent(const FSystemicEvent& Event)
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
}

// Static call to emit an event (for blueprint exposure).
void USystemicWorldSubsystem::EmitEvent(UObject* WorldContextObj, const FSystemicEvent& Event)
{
	if(!IsValid(WorldContextObj))
	{
		// Invalid world context, can't grab the subsystem.
		return;
	}
	
	TObjectPtr<USystemicWorldSubsystem> pSubsystem = WorldContextObj->GetWorld()->GetSubsystem<USystemicWorldSubsystem>();
	if(!IsValid(pSubsystem))
	{
		// Subsystem not found.
		return;
	}

	// Now emit the event.
	pSubsystem->EmitEvent(Event);
}

// Register a rule with the subsystem as an Active Rule as well as caching it off in the rule map.
bool USystemicWorldSubsystem::RegisterRule(USystemicRuleAsset* RuleIn, bool bForceActivateRule)
{
	if(!IsValid(RuleIn))
	{
		// Invalid rule; do not register.
		return false;
	}

	for(const FGameplayTag& EventTag : RuleIn->GetTriggerEventTags())
	{
		// Find or add this rule to the rule map cache for easy access by tag.
		FSystemicMappedRuleAssets& ruleAssets = RuleMap.FindOrAdd(EventTag);
		
		ruleAssets.EventTag = EventTag;
		ruleAssets.RuleAssets.Add(RuleIn);

		UE_LOG(LogJoyCoreSystems, Log, TEXT("SystemicRuleAsset added to USystemicWorldSubsystem: %s\nFull Name: %s (Event Tag: %s)."), *RuleIn->GetRuleName().ToString(), *RuleIn->GetFullName(), *EventTag.ToString());
	}

	if(bForceActivateRule)
	{
		// Override the rule's state.
		RuleIn->Enable(true);
	}

	if(RuleIn->IsEnabled())
	{
		// Rule is enabled; add to active rules.
		ActiveRules.Add(RuleIn);
		UE_LOG(LogJoyCoreSystems, Verbose, TEXT("SystemicRuleAsset (Name: %s, Event Tags: %s) activated in USystemicWorldSubsystem."), *RuleIn->GetRuleName().ToString(), *RuleIn->GetTriggerEventTags().ToString());
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

	// Load all Rule Assets.
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> assetData;
	const UClass* Class = USystemicRuleAsset::StaticClass();
	
	// Get all Rule Assets by class.
	assetRegistryModule.Get().GetAssetsByClass(Class->GetClassPathName(), assetData, true);
	
	// Go through and register all the rules with the map and the active rule list if the rule is enabled.
	for(const FAssetData& asset : assetData)
	{
		TObjectPtr<USystemicRuleAsset> pRule = Cast<USystemicRuleAsset>(asset.GetAsset());
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
}

// Clear out all the data.
void USystemicWorldSubsystem::Deinitialize()
{
	// Clear the active rule list.
	ActiveRules.Empty();
	EventQueue.Empty();
	RuleMap.Empty();
	
	Super::Deinitialize();
}