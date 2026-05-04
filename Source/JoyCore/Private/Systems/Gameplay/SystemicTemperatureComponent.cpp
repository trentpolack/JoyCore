// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicTemperatureComponent.h"

#include "GameFramework/Actor.h"

#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicTemperatureEventData.h"

USystemicTemperatureComponent::USystemicTemperatureComponent()
: Temperature(AmbientTemperature)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Modify the current temperature.
float USystemicTemperatureComponent::ModifyTemperature(float TemperatureDelta, AActor* InstigatorActor, UObject* SourceObject)
{
	return(SetTemperature(GetTemperature() + TemperatureDelta, InstigatorActor, SourceObject));
}

// Set the current temperature.
float USystemicTemperatureComponent::SetTemperature(float TemperatureIn, AActor* InstigatorActor, UObject* SourceObject)
{
	// Set the previous/modified temperature.
	const float temperaturePrevious = GetTemperature();
	Temperature = TemperatureIn;

	const float temperatureDelta = Temperature - temperaturePrevious;
	if(FMath::IsNearlyZero(temperatureDelta))
	{
		// No need to bother.
		return(Temperature);
	}

	// Emit the temperature change event and broadcast it.
	EmitTemperatureEvent(Temperature, temperaturePrevious, temperatureDelta, InstigatorActor, SourceObject);
	OnTemperatureChanged.Broadcast(GetTemperature(), temperaturePrevious, temperatureDelta, InstigatorActor);
	
	// Check for state changes as a result of the temperature change; first ignition and then freezing.
	const TScriptInterface<ISystemicTraitProvider> pTraitProvider = GetTraitProvider();
	if(pTraitProvider->HasTrait(TAG_System_Trait_State_Ignited) && (Temperature < IgnitionTemperatureThreshold))
	{
		// Temperature has cooled enough to remove ignition state.
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Ignited);

		EmitStateEvent(TAG_System_Event_Ignite_Cooled, InstigatorActor, SourceObject);
	}
	else if(pTraitProvider->HasTrait(TAG_System_Trait_State_Frozen) && (Temperature > FreezeTemperatureThreshold))
	{
		// Temperature has warmed enough to remove frozen state.
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Frozen);
		
		EmitStateEvent(TAG_System_Event_Freeze_Warmed, InstigatorActor, SourceObject);
	}
	else if(pTraitProvider->HasTrait(TAG_System_Trait_Flammable) && ((temperaturePrevious < IgnitionTemperatureThreshold) && (Temperature >= IgnitionTemperatureThreshold)))
	{
		// Temperature has increased enough to ignite the object.
		pTraitProvider->AddTrait(TAG_System_Trait_State_Ignited);
		
		// Emit the ignite event.
		EmitStateEvent(TAG_System_Event_Ignite, InstigatorActor, SourceObject);

		// Broadcast the ignite event.
		OnIgnited.Broadcast(InstigatorActor);
	}
	else if((pTraitProvider->HasTrait(TAG_System_Trait_Freezable) && (temperaturePrevious > FreezeTemperatureThreshold) && (Temperature <= FreezeTemperatureThreshold)))
	{
		// Temperature has decreased enough to freeze the object.
		pTraitProvider->AddTrait(TAG_System_Trait_State_Frozen);

		// Emit the freeze event.
		EmitStateEvent(TAG_System_Event_Freeze, InstigatorActor, SourceObject);

		// Broadcast the freeze event.
		OnFrozen.Broadcast(InstigatorActor);
	}

	return Temperature;
}

bool USystemicTemperatureComponent::EmitTemperatureEvent(float TemperatureNew, float TemperaturePrevious, float TemperatureDelta, AActor* InstigatorActor, UObject* SourceObject) const
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		return false;
	}

	// Setup the event.
	FSystemicEvent Event;
	Event.EventTag = TAG_System_Event_TemperatureChanged;
	Event.Target = pOwner;
	Event.Instigator = InstigatorActor;
	Event.SourceObject = SourceObject ? SourceObject : const_cast<USystemicTemperatureComponent*>(this);
	Event.EventDataInstance.InitializeAs<FSystemicTemperatureEventData>();

	// Fill out the temperature event data.
	FSystemicTemperatureEventData& EventData = Event.EventDataInstance.GetMutable<FSystemicTemperatureEventData>();
	EventData.Location = pOwner->GetActorLocation();
	EventData.TemperatureNew = TemperatureNew;
	EventData.TemperaturePrevious = TemperaturePrevious;
	EventData.Value = TemperatureDelta;
	
	return(USystemicWorldSubsystem::EmitEvent(pOwner, Event));
}

bool USystemicTemperatureComponent::EmitStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* SourceObject) const
{
	TObjectPtr<AActor> pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid owner.
		return false;
	}

	FSystemicEvent Event;
	Event.EventTag = EventTag;
	Event.Target = pOwner;
	Event.Instigator = InstigatorActor;
	Event.SourceObject = SourceObject ? SourceObject : const_cast<USystemicTemperatureComponent*>(this);
	Event.EventDataInstance.InitializeAs<FSystemicEventData>();

	FSystemicEventData& EventData = Event.EventDataInstance.GetMutable<FSystemicEventData>();
	EventData.Location = pOwner->GetActorLocation();
	EventData.Value = GetTemperature();

	return(USystemicWorldSubsystem::EmitEvent(pOwner, Event));
}

// Tick the component to modify temperature towards equilibrium.
void USystemicTemperatureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(FMath::IsNearlyZero(EquilibriumRate) || FMath::IsNearlyEqual(Temperature, AmbientTemperature))
	{
		// No need to modify temperature.
		return;
	}

	// Modify the temperature to bring it closer to the ambient temperature.
	ModifyTemperature(FMath::FInterpConstantTo(Temperature, AmbientTemperature, DeltaTime, EquilibriumRate) - Temperature, GetOwner(), this);
}