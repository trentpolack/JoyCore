// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicInteractionComponent.h"

#include "GameFramework/Actor.h"

#include "JoyCoreNativeGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicInteractionEventData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SystemicInteractionComponent)

// Constants.
const FName USystemicInteractionComponent::InteractionNameDefault = TEXT("Interact");

USystemicInteractionComponent::USystemicInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USystemicInteractionComponent::EmitInteractEvent(const FName& InteractionName, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Return early if component is disabled or owner is invalid (though if the component is disabled, it shouldn't get this far).
		return false;
	}

	// Populate evnet data.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, TAG_System_Event_Interacted, pOwner, InstigatorActor, Source, FSystemicInteractionEventData);

	FSystemicInteractionEventData& eventData = event.EventDataInstance.GetMutable<FSystemicInteractionEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = 1.0f;

	eventData.InteractionName = InteractionName;

	// Broadcast the interaction.
	OnInteracted.Broadcast(InstigatorActor, InteractionName);

	// Emit the Interact event.
	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

bool USystemicInteractionComponent::Interact(const FName& InteractionName, AActor* InstigatorActor, UObject* Source)
{
	if(!GetIsEnabled())
	{
		// Return early if component is disabled; though it probably shouldn't get this far.
		return false;
	}

	// Emit the interaction event with the interaction name.
	const FName& resolvedInteractionName = InteractionName.IsNone() ? InteractionNameDefault : InteractionName;
	return(EmitInteractEvent(resolvedInteractionName, InstigatorActor, Source));
}
