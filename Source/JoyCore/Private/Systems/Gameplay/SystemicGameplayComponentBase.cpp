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
	check(TraitProvider);
	return TraitProvider;
}

// Begin play logic; primarily for retrieving a valid trait provider.
void USystemicGameplayComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// Get the trait provider from the owner.
	TObjectPtr<AActor> pOwner = GetOwner();
	check(IsValid(pOwner));

	if(pOwner->Implements<USystemicTraitProvider>())
	{
		// Actor implements the interface.
		TraitProvider.SetObject(pOwner);
		TraitProvider.SetInterface(Cast<ISystemicTraitProvider>(pOwner));
	}
	else
	{
		// Look through other components to see if it's implemented.
		TraitProvider = GetOwner()->FindComponentByInterface(USystemicTraitProvider::StaticClass());
	}

	check(TraitProvider);
}