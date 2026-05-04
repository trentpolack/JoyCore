// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTags.h"

#include "SystemicGameplayComponentBase.h"

#include "SystemicHealthComponent.generated.h"

// Declarations.
class AController;
class UDamageType;

// Delegate for broadcasting health changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSystemicHealthChangedSignature, float, HealthPrevious, float, HealthNew, float, HealthDelta, AActor*, InstigatorActor);
// Delegate for broadcasting object state changes due to health changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSystemicLifeStateSignature, AActor*, InstigatorActor);

/**
 *	USystemicHealthComponent Class.
 *		Actor component that tracks health and emits health and life-state events. Can optionally support a downed state before death (default: false).
 */
UCLASS(Category="Game|Systems|Components", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicHealthComponent : public USystemicGameplayComponentBase
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicHealthComponent Constructor.
	 */
	USystemicHealthComponent();

protected:
	// Current health value.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Health|Transient", meta=(ClampMin="0.0"))
	float Health = HealthMax;

	// Maximum health value used when clamping current health.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, DisplayName="Max Health", Category="Health|Config", meta=(ClampMin="0.0"))
	float HealthMax = 100.0f;
	
	// Whether this component destroys its owner when health reaches zero (default: false).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Health|Config")
	uint8 bDestroyOwnerOnKilled = false;

	// Whether this component supports a downed state before death (default: false).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Health|Config")
	uint8 bDownedStateSupported = false;

protected:
	/**
	 *	Emit a systemic health change event.
	 *	@param HealthDelta Health delta applied.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 *	@param bWasFatal True if the change caused this component to enter the killed state.
	 */
	virtual void EmitHealthEvent(float HealthDelta, AActor* InstigatorActor, UObject* SourceObject, bool bWasFatal) const;

	/**
	 *	Emit a systemic life-state event.
	 *	@param EventTag Gameplay tag identifying the life-state event.
	 *	@param InstigatorActor Actor responsible for the state change.
	 *	@param SourceObject Object responsible for the state change.
	 */
	virtual void EmitLifeStateEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* SourceObject) const;

	/**
	 *	Handle health change events from this component's owner.
	 *	@param Actor Actor that received damage.
	 *	@param HealthDelta Health delta applied.
	 *	@param DamageType Damage type object (if relevant).
	 *	@param InstigatorActor Controller responsible for the damage (if relevant).
	 *	@param SourceObject Actor that caused the damage.
	 */
	UFUNCTION()
	virtual void OnHealthModified(AActor* Actor, float HealthDelta, const UDamageType* DamageType, AController* InstigatorActor, UObject* SourceObject);

public:
	// Broadcast when this component's health changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicHealthChangedSignature OnHealthChanged;

	// Broadcast when this component is spawned.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifeStateSignature OnSpawned;

	// Broadcast when this component crosses into a downed state.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifeStateSignature OnDowned;

	// Broadcast when this component owner is revived.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifeStateSignature OnRevived;

	// Broadcast when health is zero and the owner is already downed (if it's a supported state).
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifeStateSignature OnKilled;
	
	/**
	 *	Set this component's health.
	 *	@param HealthIn New health value.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	void SetHealth(float HealthIn, AActor* InstigatorActor = nullptr, UObject* SourceObject = nullptr);

	/**
	 *	Get this component's current health.
	 *	@return Current health value.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Health")
	float GetHealth() const { return Health; }

	/**
	 *	Set this component's max health.
	 *	@param HealthMaxIn New max health value.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 */
	UFUNCTION(BlueprintCallable, DisplayName="Set Max Health", Category="Game|Systems|Health")
	void SetHealthMax(float HealthMaxIn, AActor* InstigatorActor = nullptr, UObject* SourceObject = nullptr);

	/**
	 *	Get this component's maximum health.
	 *	@return Maximum health value.
	 */
	UFUNCTION(BlueprintPure, DisplayName="Get Max Health", Category="Game|Systems|Health")
	float GetHealthMax() const { return HealthMax; }
	
	/**
	 *	Modify the current health of this component.
	 *	@param HealthDelta Health delta to apply; negative values damage, positive values heal.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param SourceObject Object responsible for the change.
	 *	@return Modified health value.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	virtual float ModifyHealth(float HealthDelta, AActor* InstigatorActor = nullptr, UObject* SourceObject = nullptr);
	
	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~UActorComponent.
};