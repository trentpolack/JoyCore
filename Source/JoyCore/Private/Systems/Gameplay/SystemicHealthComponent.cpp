// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicHealthComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

#include "Systems/Traits/ISystemicTraitProvider.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicHealthEventData.h"

USystemicHealthComponent::USystemicHealthComponent()
: HealthMax(100.0f)
, Health(HealthMax)
, bBindToDamageEvents(true)
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Begin play and bind to damage events (if specified).
void USystemicHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(bBindToDamageEvents)
	{
		// Bind to the engine's damage event.
		AActor* pOwner = GetOwner();
		if(IsValid(pOwner))
		{
			pOwner->OnTakeAnyDamage.AddUniqueDynamic(this, &USystemicHealthComponent::HandleTakeDamage);
		}
	}
	
	// Set the alive state and ensure the response is true (indicating it wasn't already set).
	TScriptInterface<ISystemicTraitProvider> pTraitProvider = GetTraitProvider();
	ensure(pTraitProvider->AddTrait(TAG_System_Trait_State_Alive));
	
	// Broadcast the Spawn event.
	OnSpawned.Broadcast(GetOwner(), this);
}

// Cleanup and unbind from damage events.
void USystemicHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(bBindToDamageEvents)
	{
		// Unbind from the engine's damage event.
		AActor* pOwner = GetOwner();
		if(IsValid(pOwner))
		{
			pOwner->OnTakeAnyDamage.RemoveDynamic(this, &USystemicHealthComponent::HandleTakeDamage);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Engine damage response.
void USystemicHealthComponent::HandleTakeDamage(AActor* Actor, float Damage, const UDamageType* DamageType, AController* InstigatorActor, AActor* DamageCauser)
{
	if(FMath::IsNearlyZero(Damage))
	{
		// No need to bother.
		return;
	}
	
	// Get the instigator and invoke ::ModifyHealth.
	AActor* pInstigator = IsValid(InstigatorActor) ? InstigatorActor->GetPawn() : nullptr;
	ModifyHealth(-Damage, pInstigator, DamageCauser);
}

// Emit a health change event.
bool USystemicHealthComponent::EmitHealthEvent(float HealthNew, float HealthPrevious, float HealthDelta, AActor* InstigatorActor, UObject* SourceObject)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	FSystemicEvent Event;
	JOYCORE_POPULATE_EVENT(Event, TAG_System_Event_HealthChanged, pOwner, InstigatorActor, SourceObject, FSystemicHealthEventData);

	FSystemicHealthEventData& eventData = Event.EventDataInstance.GetMutable<FSystemicHealthEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.HealthNew = HealthNew;
	eventData.HealthPrevious = HealthPrevious;
	eventData.Value = HealthDelta;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, Event));
}

// Emit a health max change event.
bool USystemicHealthComponent::EmitHealthMaxEvent(float HealthMaxNew, float HealthMaxPrevious, float HealthMaxDelta, AActor* InstigatorActor, UObject* SourceObject)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	FSystemicEvent Event;
	JOYCORE_POPULATE_EVENT(Event, TAG_System_Event_HealthMaxChanged, pOwner, InstigatorActor, SourceObject, FSystemicHealthEventData);

	FSystemicHealthEventData& eventData = Event.EventDataInstance.GetMutable<FSystemicHealthEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.HealthNew = HealthMaxNew;
	eventData.HealthPrevious = HealthMaxPrevious;
	eventData.Value = HealthMaxDelta;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, Event));
}

// Emit a lifecycle change event.
bool USystemicHealthComponent::EmitLifeStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* SourceObject)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	// Fill out the health event data.
	FSystemicEvent Event;
	JOYCORE_POPULATE_EVENT(Event, EventTag, pOwner, InstigatorActor, SourceObject, FSystemicEventData);

	FSystemicEventData& eventData = Event.EventDataInstance.GetMutable<FSystemicEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = GetHealth();

	return(USystemicWorldSubsystem::EmitEvent(pOwner, Event));
}

// Set the current health.
float USystemicHealthComponent::SetHealth(float HealthIn, AActor* InstigatorActor, UObject* SourceObject)
{
	TScriptInterface<ISystemicTraitProvider> pTraitProvider = GetTraitProvider();

	// Cache the current health and update it.
	const float healthPrevious = GetHealth();
	const float clampedHealthIn = FMath::Clamp(HealthIn, 0.0f, GetHealthMax());

	// Get the delta and see if there's any need for further logic (unless in the downed state in which case it's a killing change).
	const float healthDelta = FMath::Clamp(clampedHealthIn, 0.0f, GetHealthMax()) - healthPrevious;
	if(!pTraitProvider->HasTrait(TAG_System_Trait_State_Downed) && (FMath::IsNearlyZero(healthDelta) || pTraitProvider->HasTrait(TAG_System_Trait_State_Dead)))
	{
		// No need for additional logic; health is either unchanged or this object is already dead.
		return Health;
	}

	// Assign the new health and emit the health changed events. 
	Health = clampedHealthIn;
	EmitHealthEvent(Health, healthPrevious, healthDelta, InstigatorActor, SourceObject);

	// Broadcast the change.
	OnHealthChanged.Broadcast(healthPrevious, Health, healthDelta, InstigatorActor, SourceObject);

	// Handle health-related state logic.
	if((Health <= 0.0f) && pTraitProvider->HasTrait(TAG_System_Trait_State_Alive))
	{
		// Remove the alive state; owner is either Downed or Dead.
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Alive);

		// Time for some death logic.
		if(bDownedStateSupported && !pTraitProvider->HasTrait(TAG_System_Trait_State_Downed))
		{
			// The Downed interim state before Death is supported; update the state and emit the downed event.
			pTraitProvider->AddTrait(TAG_System_Trait_State_Downed);
			
			EmitLifeStateEvent(TAG_System_Event_Downed, InstigatorActor, SourceObject);
			
			// Broadcast the Downed event.
			OnDowned.Broadcast(InstigatorActor, SourceObject);
		}
		else if((!bDownedStateSupported || pTraitProvider->HasTrait(TAG_System_Trait_State_Downed)) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Dead))
		{
			// Further damage has been received while downed or the downed state is not supported; kill the owner.
			pTraitProvider->RemoveTrait(TAG_System_Trait_State_Downed);	// In case it was downed.
			pTraitProvider->AddTrait(TAG_System_Trait_State_Dead);		// Now mark dead.

			EmitLifeStateEvent(TAG_System_Event_Killed, InstigatorActor, SourceObject);
			
			// Broadcast the Killed event.
			OnKilled.Broadcast(InstigatorActor, SourceObject);
			
			if(bDestroyOwnerOnKilled)
			{
				// Destroy the owner actor if specified.
				AActor* pOwner = GetOwner();
				if(IsValid(pOwner))
				{
					// Destruction event is triggered from USystemicTraitComponent so just destroy the owner here.
					pOwner->Destroy();
				}
			}
		}
	}
	else if((Health > 0.0f) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Alive))
	{
		// The owner is alive. Do a check to ensure that the player was downed and not dead.
		ensure(pTraitProvider->HasTrait(TAG_System_Trait_State_Downed) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Dead));
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Downed);	// Remove the downed state.

		// Add the alive state back.
		pTraitProvider->AddTrait(TAG_System_Trait_State_Alive);

		// Emit the revival event.
		EmitLifeStateEvent(TAG_System_Event_Revived, InstigatorActor, SourceObject);

		// Broadcast the revive event.
		OnRevived.Broadcast(InstigatorActor, SourceObject);
	}
	
	return Health;
}

float USystemicHealthComponent::SetHealthMax(float HealthMaxIn, AActor* InstigatorActor, UObject* SourceObject)
{
	// Set the new maximum health value (no need for an event).
	const float healthMaxPrevious = HealthMax;
	HealthMax = FMath::Max(0.01f, HealthMaxIn);
	
	const float healthMaxDelta = HealthMax - healthMaxPrevious;
	if(FMath::IsNearlyZero(healthMaxDelta))
	{
		// No need for additional logic.
		return HealthMax;
	}
	
	if(Health > HealthMax)
	{
		// Health needs to be modified to fit within the new maximum.
		SetHealth(Health, InstigatorActor, SourceObject);
	}
	
	// Emit the health changed event.
	EmitHealthMaxEvent(HealthMax, healthMaxPrevious, healthMaxDelta, InstigatorActor, SourceObject);

	// Broadcast the change.
	OnHealthMaxChanged.Broadcast(HealthMax, healthMaxPrevious, healthMaxDelta, InstigatorActor, SourceObject);
	return HealthMax;
}

// Modify and set the health.
float USystemicHealthComponent::ModifyHealth(float HealthDelta, AActor* InstigatorActor, UObject* SourceObject)
{
	return(SetHealth(GetHealth() + HealthDelta, InstigatorActor, SourceObject));
}