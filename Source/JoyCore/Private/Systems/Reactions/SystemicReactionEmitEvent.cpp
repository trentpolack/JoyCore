// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionEmitEvent.h"

#include "Engine/World.h"

#include "Systems/SystemicCore.h"
#include "Systems/SystemicWorldSubsystem.h"

void USystemicReactionEmitEvent::PopulateEventData(const FSystemicEvent& Event, FSystemicRuleContext& Context, TInstancedStruct<FSystemicEventData> EventDataOut)
{
	AActor* pActor = Cast<AActor>(Event.GetObjectBySubject(ReactionSubject));
	if(!IsValid(pActor))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Reaction subject is not a valid actor: %s"), *Event.EventTag.ToString());
		return;
	}
	
	// Base level implementation can only work off of the limited native event data structure.
	EventDataOut = ReactionEvent.EventDataStruct.Make();

	FSystemicEventData& eventData = EventDataOut.GetMutable<FSystemicEventData>();
	eventData.Location = pActor->GetActorLocation();
	eventData.Magnitude = Event.EventDataInstance->Magnitude;
}

// Execute the blueprint-implemented event as a reaction to the triggering event.
bool USystemicReactionEmitEvent::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	// Populate the event data and emit it.
	PopulateEventData(Event, Context, ReactionEvent.EventDataInstance);
	USystemicWorldSubsystem::EmitEvent(GetWorld(), ReactionEvent);
	
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, true));
	return true;
}