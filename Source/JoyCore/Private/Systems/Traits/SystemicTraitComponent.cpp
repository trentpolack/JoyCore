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
	TObjectPtr<AActor> pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Trait Component has no owner: %s"), *GetName());
		return false;
	}

	FSystemicEvent event;
	event.EventTag = EventTag;
	event.Target = pOwner;
	event.SourceObject = ConstCast<USystemicTraitComponent>(this);
	event.EventDataInstance.InitializeAs<FSystemicEventData>();

	FSystemicEventData& eventData = event.EventDataInstance.GetMutable<FSystemicEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = 0.0f;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Adds a trait tag to the component.
bool USystemicTraitComponent::AddTrait(const FGameplayTag& TraitTag)
{
	if(Traits.HasTag(TraitTag))
	{
		// Already present.
		return false;
	}

	// Add a new trait tag.
	Traits.AddTag(TraitTag);
	
	// Broadcast the tag change.
	OnTraitsChanged.Broadcast(this, GetTraits(), FGameplayTagContainer(TraitTag));
	return true;
}

// Adds trait tags to the component.
bool USystemicTraitComponent::AddTraits(const FGameplayTagContainer& TraitTagContainer)
{
	FGameplayTagContainer TagsFiltered_New;

	for(const FGameplayTag& Tag : TraitTagContainer)
	{
		if(Traits.HasTag(Tag))
		{
			// Already present.
			continue;
		}

		TagsFiltered_New.AddTag(Tag);
	}
	
	if(TagsFiltered_New.IsEmpty())
	{
		// No new tags to add.
		return false;
	}
	
	// Append only the new tags.
	Traits.AppendTags(TagsFiltered_New);
	
	// Broadcast the tag change.
	OnTraitsChanged.Broadcast(this, GetTraits(), TagsFiltered_New);
	return true;
}

// Removes a trait tag from the component.
bool USystemicTraitComponent::RemoveTraits(const FGameplayTag& TraitTag)
{
	if(!Traits.HasTag(TraitTag))
	{
		// Tag not present.
		return false;
	}
	
	// Remove the tag.
	Traits.RemoveTag(TraitTag);
	
	// Broadcast the tag change.
	OnTraitsChanged.Broadcast(this, GetTraits(), FGameplayTagContainer(TraitTag));
	return true;
}

// Remove trait tags from the component.
bool USystemicTraitComponent::RemoveTraits(const FGameplayTagContainer& TraitTagContainer)
{
	FGameplayTagContainer TagsFiltered_Removed;

	for(const FGameplayTag& Tag : TraitTagContainer)
	{
		if(!Traits.HasTag(Tag))
		{
			// The tag is not present.
			continue;
		}

		TagsFiltered_Removed.AddTag(Tag);
	}
	
	if(TagsFiltered_Removed.IsEmpty())
	{
		// No matching tags to remove.
		return false;
	}
	
	// Remove only the filtered tags.
	Traits.RemoveTags(TagsFiltered_Removed);
	
	// Broadcast the tag change.
	OnTraitsChanged.Broadcast(this, GetTraits(), TagsFiltered_Removed);
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