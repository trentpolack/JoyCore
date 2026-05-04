// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicGameplayComponentBase.h"

#include "GameFramework/Actor.h"

#include "Systems/Traits/SystemicTraitComponent.h"

// Constructor.
USystemicGameplayComponentBase::USystemicGameplayComponentBase()
: TraitProvider(nullptr)
{
}

// Get the trait provider for this component.
TScriptInterface<ISystemicTraitProvider> USystemicGameplayComponentBase::GetTraitProvider() const
{
	ensure(TraitProvider);
	return TraitProvider;
}

// Begin play logic; primarily for retrieving a valid trait provider.
void USystemicGameplayComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// Get the trait provider from the owner.
	TraitProvider = Cast<ISystemicTraitProvider>(GetOwner());
	if(!TraitProvider)
	{
		// Look through other components to see if it's implemented.
		TraitProvider = Cast<ISystemicTraitProvider>(GetOwner()->FindComponentByInterface(USystemicTraitProvider::StaticClass()));
		
		ensure(TraitProvider);
	}
}