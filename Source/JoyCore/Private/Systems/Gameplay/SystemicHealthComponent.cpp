// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicHealthComponent.h"

#include "GameFramework/Controller.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicHealthEventData.h"

USystemicHealthComponent::USystemicHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USystemicHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = FMath::Max(0.0f, MaxHealth);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	bKilled = Health <= 0.0f;

	if(bListenForEngineDamage && IsValid(GetOwner()))
	{
		GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &USystemicHealthComponent::HandleOwnerTakeAnyDamage);
	}
}

void USystemicHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(IsValid(GetOwner()))
	{
		GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &USystemicHealthComponent::HandleOwnerTakeAnyDamage);
	}

	Super::EndPlay(EndPlayReason);
}

float USystemicHealthComponent::ApplyHealthDelta(float Delta, AActor* InstigatorActor, UObject* SourceObject)
{
	SetHealth(Health + Delta, InstigatorActor, SourceObject);
	return Health;
}

float USystemicHealthComponent::ApplyDamage(float DamageAmount, AActor* InstigatorActor, UObject* SourceObject)
{
	return ApplyHealthDelta(-FMath::Max(0.0f, DamageAmount), InstigatorActor, SourceObject);
}

float USystemicHealthComponent::Heal(float HealAmount, AActor* InstigatorActor, UObject* SourceObject)
{
	return ApplyHealthDelta(FMath::Max(0.0f, HealAmount), InstigatorActor, SourceObject);
}

void USystemicHealthComponent::SetHealth(float NewHealth, AActor* InstigatorActor, UObject* SourceObject)
{
	const float PreviousHealth = Health;
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const float Delta = Health - PreviousHealth;

	if(FMath::IsNearlyZero(Delta))
	{
		return;
	}

	const bool bWasAlive = PreviousHealth > 0.0f;
	const bool bIsFatal = bWasAlive && Health <= 0.0f;

	OnHealthChanged.Broadcast(PreviousHealth, Health, Delta, InstigatorActor);
	EmitHealthEvent(PreviousHealth, Health, Delta, InstigatorActor, SourceObject, bIsFatal);

	if(bIsFatal)
	{
		bKilled = true;
		OnDowned.Broadcast(InstigatorActor);
		OnKilled.Broadcast(InstigatorActor);
		EmitLifeStateEvent(TAG_System_Event_Downed, InstigatorActor, SourceObject);
		EmitLifeStateEvent(TAG_System_Event_Killed, InstigatorActor, SourceObject);

		if(bDestroyOwnerOnKilled && IsValid(GetOwner()))
		{
			GetOwner()->Destroy();
		}
	}
	else if(PreviousHealth <= 0.0f && Health > 0.0f)
	{
		bKilled = false;
		EmitLifeStateEvent(TAG_System_Event_Revived, InstigatorActor, SourceObject);
	}
}

void USystemicHealthComponent::HandleOwnerTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	AActor* InstigatorActor = IsValid(InstigatedBy) ? InstigatedBy->GetPawn() : nullptr;
	ApplyDamage(Damage, InstigatorActor, DamageCauser);
}

void USystemicHealthComponent::EmitHealthEvent(float PreviousHealthValue, float NewHealth, float Delta, AActor* InstigatorActor, UObject* SourceObject, bool bWasFatal) const
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		return;
	}

	FSystemicEvent Event;
	Event.EventTag = TAG_System_Event_HealthChanged;
	Event.Target = Owner;
	Event.Instigator = InstigatorActor;
	Event.SourceObject = SourceObject ? SourceObject : const_cast<USystemicHealthComponent*>(this);
	Event.EventDataInstance.InitializeAs<FSystemicHealthEventData>();

	FSystemicHealthEventData& EventData = Event.EventDataInstance.GetMutable<FSystemicHealthEventData>();
	EventData.Location = Owner->GetActorLocation();
	EventData.Magnitude = FMath::Abs(Delta);
	EventData.PreviousHealth = PreviousHealthValue;
	EventData.NewHealth = NewHealth;
	EventData.MaxHealth = MaxHealth;
	EventData.Delta = Delta;
	EventData.bWasFatal = bWasFatal;

	USystemicWorldSubsystem::EmitEvent(Owner, Event);
}

void USystemicHealthComponent::EmitLifeStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* SourceObject) const
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		return;
	}

	FSystemicEvent Event;
	Event.EventTag = EventTag;
	Event.Target = Owner;
	Event.Instigator = InstigatorActor;
	Event.SourceObject = SourceObject ? SourceObject : const_cast<USystemicHealthComponent*>(this);
	Event.EventDataInstance.InitializeAs<FSystemicHealthEventData>();

	FSystemicHealthEventData& EventData = Event.EventDataInstance.GetMutable<FSystemicHealthEventData>();
	EventData.Location = Owner->GetActorLocation();
	EventData.Magnitude = Health;
	EventData.PreviousHealth = Health;
	EventData.NewHealth = Health;
	EventData.MaxHealth = MaxHealth;

	USystemicWorldSubsystem::EmitEvent(Owner, Event);
}
