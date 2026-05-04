// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/Traits/ISystemicTraitProvider.h"

#include "SystemicGameplayComponentBase.h"

#include "SystemicTemperatureComponent.generated.h"

// Delegate for broadcasting temperature changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSystemicTemperatureChangedSignature, float, TemperatureNew, float, TemperaturePrevious, float, TemperatureDelta, AActor*, InstigatorActor);
// Delegate for broadcasting objecet state changes due to temperature changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSystemicTemperatureStateSignature, AActor*, InstigatorActor);

/**
 *	USystemicTemperatureComponent Class.
 *		Actor component that tracks temperature and emits systemic temperature and state events.
 *		The intended temperature unit is Degrees Celsius (°C).
 */
UCLASS(Category="Game|Systems|Components", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicTemperatureComponent : public USystemicGameplayComponentBase
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicTemperatureComponent Constructor.
	 */
	USystemicTemperatureComponent();

protected:
	// Current temperature value.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Temperature|Transient")
	float Temperature = 0.0f;

	// Ambient temperature value used for equilibrium behavior.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Temperature|Config")
	float AmbientTemperature = 20.0f;

	// Temperature at or above which this component becomes ignited.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Temperature|Config")
	float IgnitionTemperatureThreshold = 220.0f;

	// Temperature at or below which this component becomes frozen.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Temperature|Config")
	float FreezeTemperatureThreshold = 0.0f;

	// Rate in Degrees Celsius (°C) per second at which current temperature moves toward ambient temperature (default: 0.1 °C).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Temperature|Config", meta=(ClampMin="0.0"))
	float EquilibriumRate = 0.1f;

protected:
	/**
	 *	Emit a systemic temperature change event.
	 *	@param TemperatureNew Temperature after the change.
	 *	@param TemperaturePrevious Temperature before the change.
	 *	@param TemperatureDelta Temperature delta applied.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 *	@return Returns true if the event is successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Temperature")
	bool EmitTemperatureEvent(float TemperatureNew, float TemperaturePrevious, float TemperatureDelta, AActor* InstigatorActor, UObject* SourceObject) const;

	/**
	 *	Emit a systemic temperature state event.
	 *	@param EventTag Gameplay tag identifying the state event.
	 *	@param InstigatorActor Actor responsible for the state change.
	 *	@param SourceObject Object responsible for the state change.
	 *	@return Returns true if the event is successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Temperature", meta=(GameplayTagFilter="System.Trait.State"))
	bool EmitStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* SourceObject) const;

public:
	// Broadcast when this component's temperature changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicTemperatureChangedSignature OnTemperatureChanged;

	// Broadcast when this component crosses into an ignited state.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicTemperatureStateSignature OnIgnited;

	// Broadcast when this component crosses into a frozen state.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicTemperatureStateSignature OnFrozen;

	/**
	 *	Set this component's temperature.
	 *	@param TemperatureIn New temperature value.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 *	@return The temperature after modification.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Temperature")
	float SetTemperature(float TemperatureIn, AActor* InstigatorActor = nullptr, UObject* SourceObject = nullptr);

	/**
	 *	Get this component's current temperature.
	 *	@return Current temperature value.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Temperature")
	float GetTemperature() const { return Temperature; }

	/**
	 *	Apply a temperature delta to this component.
	 *	@param TemperatureDelta Temperature delta to apply.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 *	@return New temperature value.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	float ModifyTemperature(float TemperatureDelta, AActor* InstigatorActor = nullptr, UObject* SourceObject = nullptr);

	/**
	 *	Check whether this component is currently ignited.
	 *	@return True if ignited, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Temperature", meta=(GameplayTagFilter="System.Trait.State"))
	bool IsIgnited() const { return(GetTraitProvider()->HasTrait(TAG_System_Trait_State_Ignited)); }

	/**
	 *	Check whether this component is currently frozen.
	 *	@return True if frozen, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Temperature")
	bool IsFrozen() const { return(GetTraitProvider()->HasTrait(TAG_System_Trait_State_Frozen)); }

	// UActorComponent.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ~UActorComponent.
};
