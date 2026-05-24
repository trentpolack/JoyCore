// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Traits/SystemicTraitComponent.h"

#include "GameFramework/Actor.h"
#include "Systems/SystemicCore.h"

#include "Systems/Events/SystemicEvent.h"
#include "Systems/SystemicGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

// Constructor.
USystemicTraitComponent::USystemicTraitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Emit a lifecycle event for this component (created/destroyed).
bool USystemicTraitComponent::EmitLifecycleEvent(const FGameplayTag& EventTag)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Trait Component has no owner: %s"), *GetName());
		return false;
	}

	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, EventTag, pOwner, pOwner, this, FSystemicEventData);

	FSystemicEventData& eventData = event.GetEventDataMutable<FSystemicEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = 0.0f;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Broadcast OnTraitsChanged and, for state changes, also broadcast OnStateChanged.
bool USystemicTraitComponent::BroadcastEvents(UObject* Object, const FGameplayTagContainer& TraitTags, const FGameplayTagContainer& TraitTagsNew, const FGameplayTagContainer& TraitTagsRemoved)
{
	// Broadcast the tag change delegate.
	OnTraitsChanged.Broadcast(this, GetTraits(), TraitTagsNew, TraitTagsRemoved);
	
	// Broadcast the systemic subsystem event.
	FSystemicEvent traitsChangedEvent;
	JOYCORE_POPULATE_EVENT(traitsChangedEvent, TAG_System_Event_TraitsChanged, Object, Cast<AActor>(Object), this, FSystemicEventData);

	// Try and get a location from the owner.
	FVector location = FVector::ZeroVector;
	if(const AActor* pActor = Cast<AActor>(Object))
	{
		// Get actor location.
		location = pActor->GetActorLocation();
	}
	else if(const USceneComponent *pComponent = Cast<USceneComponent>(Object))
	{
		// Get the component location.
		location = pComponent->GetComponentLocation();
	}
	
	{
		// Fill out trait change event data.
		FSystemicEventData& eventData = traitsChangedEvent.GetEventDataMutable<FSystemicEventData>();
		eventData.Location = location;
		eventData.Value = 0.0f;
	}
	
	// Emit the TraitsChanged event.
	bool result = USystemicWorldSubsystem::EmitEvent(Object, traitsChangedEvent);
	if(TraitTagsNew.HasTag(TAG_System_Trait_State) || TraitTagsRemoved.HasTag(TAG_System_Trait_State))
	{
		FSystemicEvent stateChangedEvent;
		JOYCORE_POPULATE_EVENT(stateChangedEvent, TAG_System_Event_StateChanged, Object, Cast<AActor>(Object), this, FSystemicEventData);
		
		{
			// Fill out state change event data.
			FSystemicEventData& eventData = stateChangedEvent.GetEventDataMutable<FSystemicEventData>();
			eventData.Location = location;
			eventData.Value = 0.0f;
		}
		
		// Broadcast the state change delegate.
		OnStateChanged.Broadcast(this, GetTraits(), TraitTagsNew.Filter(FGameplayTagContainer(TAG_System_Trait_State)), TraitTagsRemoved.Filter(FGameplayTagContainer(TAG_System_Trait_State)));
		
		// Emit the StateChanged event.
		result = result && USystemicWorldSubsystem::EmitEvent(Object, stateChangedEvent);
	}
	
	return result;
}

// Adds a trait tag to the component.
bool USystemicTraitComponent::AddTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent)
{
	if(Traits.HasTagExact(TraitTag))
	{
		// Already present.
		return false;
	}

	// Add a new trait tag.
	Traits.AddTag(TraitTag);
	
	if(bEmitTraitChangedEvent)
	{
		// Broadcast the relevant events.
		BroadcastEvents(GetOwner(), GetTraits(), FGameplayTagContainer(TraitTag), FGameplayTagContainer());
	}
	
	return true;
}

// Adds trait tags to the component.
const FGameplayTagContainer USystemicTraitComponent::AddTraits(const FGameplayTagContainer& TraitTagContainer, bool bEmitTraitChangedEvent)
{
	FGameplayTagContainer tagsFiltered_New;

	// Go through the passed-in tag container and only find new tags.
	for(const FGameplayTag& Tag : TraitTagContainer)
	{
		if(Traits.HasTagExact(Tag))
		{
			// Already present.
			continue;
		}

		tagsFiltered_New.AddTag(Tag);
	}
	
	if(tagsFiltered_New.IsEmpty())
	{
		// No new tags to add.
		return(FGameplayTagContainer());
	}
	
	// Append only the new tags.
	Traits.AppendTags(tagsFiltered_New);
	
	if(bEmitTraitChangedEvent)
	{
		// Broadcast the relevant events.
		BroadcastEvents(GetOwner(), GetTraits(), tagsFiltered_New, FGameplayTagContainer());
	}

	return tagsFiltered_New;
}

// Removes a trait tag from the component.
bool USystemicTraitComponent::RemoveTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent)
{
	if(!Traits.HasTagExact(TraitTag))
	{
		// Tag not present.
		return false;
	}
	
	// Remove the tag.
	Traits.RemoveTag(TraitTag);

	if(bEmitTraitChangedEvent)
	{
		// Broadcast the relevant events.
		BroadcastEvents(GetOwner(), GetTraits(), FGameplayTagContainer(), FGameplayTagContainer(TraitTag));
	}
	
	return true;
}

// Remove trait tags from the component.
const FGameplayTagContainer USystemicTraitComponent::RemoveTraits(const FGameplayTagContainer& TraitTagContainer, bool bEmitTraitChangedEvent)
{
	FGameplayTagContainer tagsFiltered_Removed;

	// Go through the passed-in tag container and only find existing tags to remove.
	for(const FGameplayTag& Tag : TraitTagContainer)
	{
		if(!Traits.HasTagExact(Tag))
		{
			// The tag is not present.
			continue;
		}

		tagsFiltered_Removed.AddTag(Tag);
	}
	
	if(tagsFiltered_Removed.IsEmpty())
	{
		// No matching tags to remove.
		return(FGameplayTagContainer());
	}
	
	// Remove only the filtered tags.
	Traits.RemoveTags(tagsFiltered_Removed);
	
	if(bEmitTraitChangedEvent)
	{
		// Broadcast the relevant events.
		BroadcastEvents(GetOwner(), GetTraits(), FGameplayTagContainer(), tagsFiltered_Removed);
	}

	return tagsFiltered_Removed;
}

// Add/remove traits from the current container's tags; intended for batch changes.
bool USystemicTraitComponent::ModifyTraits(const FGameplayTagContainer& TraitTagsToAdd, const FGameplayTagContainer& TraitTagsToRemove)
{
	// Add/remove traits and prevent OnTraitsChanged event from being broadcast (so this method can just broadcast a single batch op).
	FGameplayTagContainer tagsFiltered_New(AddTraits(TraitTagsToAdd, false)), tagsFiltered_Removed(RemoveTraits(TraitTagsToRemove, false));
	
	if(!tagsFiltered_New.IsEmpty() || !tagsFiltered_Removed.IsEmpty())
	{
		// No traits were changed.
		return false;
	}
	
	// Broadcast the relevant events.
	BroadcastEvents(GetOwner(), GetTraits(), tagsFiltered_New, tagsFiltered_Removed);

	return true;
}

// Gets the component's trait tags.
const FGameplayTagContainer& USystemicTraitComponent::GetTraits() const
{
	return Traits;
}

// Called when the game starts.
void USystemicTraitComponent::BeginPlay()
{
	Super::BeginPlay();

	if(bEmitLifecycleEvents)
	{
		// Emit a creation event.
		EmitLifecycleEvent(TAG_System_Event_Created);
	}
}

// Called on game end (the game or the end of this fella).
void USystemicTraitComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(bEmitLifecycleEvents)
	{
		// Emit a destruction event.
		EmitLifecycleEvent(TAG_System_Event_Destroyed);
	}

	Super::EndPlay(EndPlayReason);
}