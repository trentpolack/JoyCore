// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Traits/SystemicTraitComponent.h"

// Constructor.
USystemicTraitComponent::USystemicTraitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts.
void USystemicTraitComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Adds a trait tag to the component.
void USystemicTraitComponent::AddTraitTag(const FGameplayTag& Tag)
{
	TraitTags.AddTag(Tag);
}

// Adds trait tags to the component.
void USystemicTraitComponent::AddTraitTags(const FGameplayTagContainer& TagContainer)
{
	TraitTags.AppendTags(TagContainer);
}

// Gets the component's trait tags.
const FGameplayTagContainer& USystemicTraitComponent::GetTraitTags() const
{
	return TraitTags;
}