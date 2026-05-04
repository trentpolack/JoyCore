// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicInteractionComponent.h"

#include "GameFramework/Actor.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicInteractionEventData.h"

// Constants.
const FName USystemicInteractionComponent::InteractionNameDefault = TEXT("Interact");

USystemicInteractionComponent::USystemicInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USystemicInteractionComponent::Interact(AActor* InstigatorActor, FName InteractionName, UObject* SourceObject)
{
	AActor* Owner = GetOwner();
	if(!GetIsEnabled() || !IsValid(Owner))
	{
		return false;
	}

	const FName ResolvedInteractionName = InteractionName.IsNone() ? InteractionNameDefault : InteractionName;
	OnInteracted.Broadcast(InstigatorActor, ResolvedInteractionName);

	FSystemicEvent Event;
	Event.EventTag = TAG_System_Event_Interacted;
	Event.Target = Owner;
	Event.Instigator = InstigatorActor;
	Event.SourceObject = SourceObject ? SourceObject : this;
	Event.EventDataInstance.InitializeAs<FSystemicInteractionEventData>();

	FSystemicInteractionEventData& EventData = Event.EventDataInstance.GetMutable<FSystemicInteractionEventData>();
	EventData.Location = Owner->GetActorLocation();
	EventData.Value = 1.0f;

	EventData.InteractionName = ResolvedInteractionName;

	USystemicWorldSubsystem::EmitEvent(Owner, Event);
	return true;
}
