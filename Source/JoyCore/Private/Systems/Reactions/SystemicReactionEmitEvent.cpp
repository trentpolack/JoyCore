// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionEmitEvent.h"

#include "Engine/World.h"

#include "Systems/SystemicCore.h"
#include "Systems/SystemicWorldSubsystem.h"

// Populate the event data structure with relevant information.
void USystemicReactionEmitEvent::PopulateEventData(const FSystemicEvent& Event, FSystemicRuleContext& Context, TInstancedStruct<FSystemicEventData>& EventDataOut, const UScriptStruct* EventDataStruct)
{
	AActor* pActor = Cast<AActor>(USystemicCore::GetEventObjectBySubject(Event, EventSubject));
	if(!IsValid(pActor))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Reaction subject is not a valid actor: %s"), *Event.EventTag.ToString());
		return;
	}
	
	// Base level implementation can only work off of the limited native event data structure.
	EventDataOut.InitializeAsScriptStruct(EventDataStruct ? EventDataStruct : FSystemicEventData::StaticStruct());

	FSystemicEventData& eventData = EventDataOut.GetMutable<FSystemicEventData>();
	eventData.Location = pActor->GetActorLocation();
	eventData.Value = Event.EventDataInstance->Value;
}

// Execute the blueprint-implemented event as a reaction to the triggering event.
bool USystemicReactionEmitEvent::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	AActor* pReactionActor = Cast<AActor>(USystemicCore::GetEventObjectBySubject(Event, EventSubject));
	if(!IsValid(pReactionActor))
	{
		Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, false));
		return false;
	}

	FSystemicEvent& event = ReactionEvent.GetMutable<FSystemicEvent>();
	event.Target = pReactionActor;
	event.Instigator = Event.Instigator;
	event.SourceObject = this;

	// Populate the event data and emit it.
	PopulateEventData(Event, Context, event.EventDataInstance);
	USystemicWorldSubsystem::EmitEvent(pReactionActor, ReactionEvent.GetMutable<FSystemicEvent>());
	
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, true));
	return true;
}
