// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionEmitEvent.h"

#include "Engine/World.h"

#include "Systems/SystemicCore.h"
#include "Systems/SystemicWorldSubsystem.h"

// Populate the event data structure with relevant information.
void USystemicReactionEmitEvent::PopulateEventData(const FSystemicEvent& Event, FSystemicRuleContext& Context, TInstancedStruct<FSystemicEventData>& EventDataOut)
{
	AActor* pActor = Cast<AActor>(Event.GetObjectBySubject(ReactionSubject).Get());
	if(!IsValid(pActor))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Reaction subject is not a valid actor: %s"), *Event.EventTag.ToString());
		return;
	}
	
	// Base level implementation can only work off of the limited native event data structure.
	EventDataOut = ReactionEvent.EventDataStruct.Make();

	FSystemicEventData& EventData = EventDataOut.GetMutable<FSystemicEventData>();
	EventData.Location = pActor->GetActorLocation();
	EventData.Value = Event.EventDataInstance->Value;
}

// Execute the blueprint-implemented event as a reaction to the triggering event.
bool USystemicReactionEmitEvent::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	AActor* pReactionActor = Cast<AActor>(Event.GetObjectBySubject(ReactionSubject).Get());
	if(!IsValid(pReactionActor))
	{
		Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, false));
		return false;
	}

	ReactionEvent.Target = pReactionActor;
	ReactionEvent.Instigator = Event.Instigator;
	ReactionEvent.SourceObject = this;

	// Populate the event data and emit it.
	PopulateEventData(Event, Context, ReactionEvent.EventDataInstance);
	USystemicWorldSubsystem::EmitEvent(pReactionActor, ReactionEvent);
	
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, true));
	return true;
}
