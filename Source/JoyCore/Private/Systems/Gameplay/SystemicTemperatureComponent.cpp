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
float USystemicTemperatureComponent::ModifyTemperature(float TemperatureDelta, AActor* InstigatorActor, UObject* Source)
{
	return(SetTemperature(GetTemperature() + TemperatureDelta, InstigatorActor, Source));
}

// Set the current temperature.
float USystemicTemperatureComponent::SetTemperature(float TemperatureIn, AActor* InstigatorActor, UObject* Source)
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
	EmitTemperatureEvent(Temperature, temperaturePrevious, temperatureDelta, InstigatorActor, Source);
	OnTemperatureChanged.Broadcast(GetTemperature(), temperaturePrevious, temperatureDelta, InstigatorActor);
	
	// Check for state changes as a result of the temperature change; first ignition and then freezing.
	const TScriptInterface<ISystemicTraitProvider> pTraitProvider = GetTraitProvider();
	if(pTraitProvider->HasTrait(TAG_System_Trait_State_Ignited) && (Temperature < IgnitionTemperatureThreshold))
	{
		// Temperature has cooled enough to remove ignition state.
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Ignited);
	}
	else if(pTraitProvider->HasTrait(TAG_System_Trait_State_Frozen) && (Temperature > FreezeTemperatureThreshold))
	{
		// Temperature has warmed enough to remove frozen state.
		pTraitProvider->RemoveTrait(TAG_System_Trait_State_Frozen);
	}
	else if(pTraitProvider->HasTrait(TAG_System_Trait_Flammable) && ((temperaturePrevious < IgnitionTemperatureThreshold) && (Temperature >= IgnitionTemperatureThreshold)))
	{
		// Temperature has increased enough to ignite the object.
		pTraitProvider->AddTrait(TAG_System_Trait_State_Ignited);

		// Broadcast the ignition event.
		OnIgnited.Broadcast(InstigatorActor);
	}
	else if((pTraitProvider->HasTrait(TAG_System_Trait_Freezable) && (temperaturePrevious > FreezeTemperatureThreshold) && (Temperature <= FreezeTemperatureThreshold)))
	{
		// Temperature has decreased enough to freeze the object.
		pTraitProvider->AddTrait(TAG_System_Trait_State_Frozen);

		// Broadcast the freeze event.
		OnFrozen.Broadcast(InstigatorActor);
	}

	return Temperature;
}

bool USystemicTemperatureComponent::EmitTemperatureEvent(float TemperatureNew, float TemperaturePrevious, float TemperatureDelta, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		return false;
	}

	// Setup the temperature change event.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, TAG_System_Event_TemperatureChanged, pOwner, InstigatorActor, Source, FSystemicTemperatureEventData);

	FSystemicTemperatureEventData& eventData = event.GetEventDataMutable<FSystemicTemperatureEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.TemperatureNew = TemperatureNew;
	eventData.TemperaturePrevious = TemperaturePrevious;
	eventData.Value = TemperatureDelta;
	
	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

bool USystemicTemperatureComponent::EmitStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* Source)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner))
	{
		// Invalid owner.
		return false;
	}

	// Populate the event with temperature state event data.
	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT(event, EventTag, pOwner, InstigatorActor, Source, FSystemicEventData);

	FSystemicEventData& eventData = event.GetEventDataMutable<FSystemicEventData>();
	eventData.Location = pOwner->GetActorLocation();
	eventData.Value = GetTemperature();

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
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