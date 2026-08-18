// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicHealthComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

#include "Systems/Traits/ISystemicTraitProvider.h"

#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicHealthEventData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SystemicHealthComponent)

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
	ensure(pTraitProvider->AddTrait(TAG_System_Trait_State_Lifecycle_Alive));
	
	// Broadcast the general lifecycle state change.
	OnLifecycleChanged.Broadcast(TAG_System_Trait_State_Lifecycle_Alive, GetOwner(), this);
	
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
void USystemicHealthComponent::HandleTakeDamage(AActor* Actor, const float Damage, const UDamageType* DamageType, AController* InstigatorActor, AActor* DamageCauser)
{
	if(FMath::IsNearlyZero(Damage))
	{
		// No need to bother.
		return;
	}
	
	// Get the instigator and invoke ::ModifyHealth (fall back on DamageCauser if the instigator is invalid).
	AActor* pInstigator = IsValid(InstigatorActor) ? InstigatorActor->GetPawn() : DamageCauser;
	ModifyHealth(-Damage, pInstigator, DamageCauser);
}

// Emit a health change event.
bool USystemicHealthComponent::EmitHealthEvent(const float HealthNew, const float HealthPrevious, const float HealthDelta, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	// Populate event data.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, TAG_System_Event_HealthChanged, pOwner, InstigatorActor, Source, FSystemicHealthEventData);

	FSystemicHealthEventData& eventData = event.GetEventDataMutable<FSystemicHealthEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.HealthNew = HealthNew;
	eventData.HealthPrevious = HealthPrevious;
	eventData.Value = HealthDelta;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Emit a health max change event.
bool USystemicHealthComponent::EmitHealthMaxEvent(const float HealthMaxNew, const float HealthMaxPrevious, const float HealthMaxDelta, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	// Populate event data.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, TAG_System_Event_HealthMaxChanged, pOwner, InstigatorActor, Source, FSystemicHealthEventData);

	FSystemicHealthEventData& eventData = event.GetEventDataMutable<FSystemicHealthEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.HealthNew = HealthMaxNew;
	eventData.HealthPrevious = HealthMaxPrevious;
	eventData.Value = HealthMaxDelta;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Emit a lifecycle change event.
bool USystemicHealthComponent::EmitLifecycleEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid.
		return false;
	}

	// Populate event data.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, EventTag, pOwner, InstigatorActor, Source, FSystemicEventData);

	FSystemicEventData& eventData = event.GetEventDataMutable<FSystemicEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = GetHealth();
	
	// Broadcast the general lifecycle state change.
	OnLifecycleChanged.Broadcast(EventTag, InstigatorActor, Source);

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Set the current health.
float USystemicHealthComponent::SetHealth(const float HealthIn, AActor* InstigatorActor, UObject* Source)
{
	TScriptInterface<ISystemicTraitProvider> pTraitProvider = GetTraitProvider();

	// Cache the current health and update it.
	const float healthPrevious = GetHealth();
	const float clampedHealthIn = FMath::Clamp(HealthIn, 0.0f, GetHealthMax());

	// Get the delta and see if there's any need for further logic (unless in the downed state in which case it's a killing change).
	const float healthDelta = FMath::Clamp(clampedHealthIn, 0.0f, GetHealthMax()) - healthPrevious;
	if(!pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Downed) && (FMath::IsNearlyZero(healthDelta) || pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Dead)))
	{
		// No need for additional logic; health is either unchanged or this object is already dead.
		return Health;
	}

	// Assign the new health and emit the health changed events. 
	Health = clampedHealthIn;
	EmitHealthEvent(Health, healthPrevious, healthDelta, InstigatorActor, Source);

	// Broadcast the change.
	OnHealthChanged.Broadcast(healthPrevious, Health, healthDelta, InstigatorActor, Source);

	// Handle health-related state logic.
	if((Health <= 0.0f) && pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Alive))
	{
		// Time for some death logic.
		if(bDownedStateSupported && !pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Downed))
		{
			// The Downed interim state before Death is supported; update the state and emit the downed event.
			pTraitProvider->ModifyTraits(FGameplayTagContainer(TAG_System_Trait_State_Lifecycle_Downed), FGameplayTagContainer(TAG_System_Trait_State_Lifecycle_Alive));
			
			EmitLifecycleEvent(TAG_System_Event_Lifecycle_Downed, InstigatorActor, Source);
			
			// Broadcast the Downed event.
			OnDowned.Broadcast(InstigatorActor, Source);
		}
		else if((!bDownedStateSupported || pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Downed)) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Dead))
		{
			// Further damage has been received while downed, or the downed state is not supported; kill the owner.
			{
				FGameplayTagContainer removeTags(TAG_System_Trait_State_Lifecycle_Downed);
				removeTags.AddTag(TAG_System_Trait_State_Lifecycle_Alive);
				pTraitProvider->ModifyTraits(FGameplayTagContainer(TAG_System_Trait_State_Lifecycle_Dead), removeTags);
			}

			EmitLifecycleEvent(TAG_System_Event_Lifecycle_Killed, InstigatorActor, Source);
			
			// Broadcast the Killed event.
			OnKilled.Broadcast(InstigatorActor, Source);
			
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
		else
		{
			// No batch tag change operation; remove the single Alive trait.
			pTraitProvider->RemoveTrait(TAG_System_Trait_State_Lifecycle_Alive);
		}
	}
	else if((Health > 0.0f) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Alive))
	{
		// The owner is alive. Do a check to ensure that the player was downed and not dead.
		ensure(pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Downed) && !pTraitProvider->HasTrait(TAG_System_Trait_State_Lifecycle_Dead));
		pTraitProvider->ModifyTraits(FGameplayTagContainer(TAG_System_Trait_State_Lifecycle_Alive), FGameplayTagContainer(TAG_System_Trait_State_Lifecycle_Downed));
		
		// Emit the revival event.
		EmitLifecycleEvent(TAG_System_Event_Lifecycle_Revived, InstigatorActor, Source);

		// Broadcast the revival event.
		OnRevived.Broadcast(InstigatorActor, Source);
	}
	
	return Health;
}

float USystemicHealthComponent::SetHealthMax(float HealthMaxIn, AActor* InstigatorActor, UObject* Source)
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
		SetHealth(Health, InstigatorActor, Source);
	}
	
	// Emit the health changed event.
	EmitHealthMaxEvent(HealthMax, healthMaxPrevious, healthMaxDelta, InstigatorActor, Source);

	// Broadcast the change.
	OnHealthMaxChanged.Broadcast(HealthMax, healthMaxPrevious, healthMaxDelta, InstigatorActor, Source);
	return HealthMax;
}

// Modify and set the health.
float USystemicHealthComponent::ModifyHealth(float HealthDelta, AActor* InstigatorActor, UObject* Source)
{
	return(SetHealth(GetHealth() + HealthDelta, InstigatorActor, Source));
}