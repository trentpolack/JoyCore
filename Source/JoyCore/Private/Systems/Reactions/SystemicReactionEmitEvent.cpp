// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionEmitEvent.h"

#include "Engine/World.h"

#include "Systems/SystemicCore.h"
#include "Systems/SystemicWorldSubsystem.h"
#include "Systems/Rules/SystemicRuleContext.h"

// Base implementation for ::EmitEvent; intended to be implemented by derived classes in blueprint.
void USystemicReactionEmitEvent::CreateReactionEvent_Implementation(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace, FSystemicEvent& ReactionEventOut)
{
	UObject* pObject = USystemicCore::GetEventObjectBySubject(Event, Event.Subject);
	if(!IsValid(pObject) && !IsValid(pObject = USystemicCore::GetEventObjectBySubject(Event, ReactionEventSubject)))
	{
		// Log the error but continue.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("%s subject and reaction subject are not valid objects (ReactionEmitEvent)."), *Event.EventTag.ToString());
	}
	
	// Create the reaction event.
	ReactionEventOut.EventTag = ReactionEventTag;
	ReactionEventOut.Priority = ReactionEventPriorityTag;
	ReactionEventOut.Subject = ReactionEventSubject;
	ReactionEventOut.Source = IsValid(pObject) ? pObject : Cast<UObject>(this);
	ReactionEventOut.Instigator = Event.Instigator;
	ReactionEventOut.Target = USystemicCore::GetEventObjectBySubject(Event, ReactionEventSubject);

	// Base level implementation can only fill in the basic data (FSystemicEventData properties).
	ReactionEventOut.EventDataInstance.InitializeAs(ReactionEventDataStruct.IsValid() ? ReactionEventDataStruct.GetScriptStruct() : FSystemicEventData::StaticStruct());

	const FSystemicEventData& eventData = Event.EventDataInstance.Get<FSystemicEventData>();
	FSystemicEventData& reactionEventData = ReactionEventOut.GetEventDataMutable<FSystemicEventData>();
	AActor* pActor = Cast<AActor>(USystemicCore::GetEventObjectBySubject(ReactionEventOut, ReactionEventSubject));
	reactionEventData.Location = IsValid(pActor) ? pActor->GetActorLocation() : eventData.Location;
	reactionEventData.Value = eventData.Value;
}

// Execute the blueprint-implemented event as a reaction to the triggering event.
bool USystemicReactionEmitEvent::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	// Create the reaction event.
	FSystemicEvent reactionEvent;
	CreateReactionEvent(Event, Context, Trace, reactionEvent);

	// Get the subject actor.
	UObject* pReactionSubject = Cast<UObject>(USystemicCore::GetEventObjectBySubject(reactionEvent, reactionEvent.Subject));
	if(!IsValid(pReactionSubject))
	{
		// Log the error and bail.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicReactionEmitEvent::Execute failed subject object validity check (event: %s)."), *reactionEvent.EventTag.ToString());

		Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(GetReactionName(), false));
		return false;
	}

	// Fill out additional context.
	Context.NamedObjects.Add(*FString::Printf(TEXT("ReactionEmitEventActor (%s)"), *ReactionEventTag.GetTagName().ToString()), pReactionSubject);

	// Emit it.
	USystemicWorldSubsystem::EmitEvent(pReactionSubject, reactionEvent);

	// Update the trace.
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(GetReactionName(), true));
	return true;
}
