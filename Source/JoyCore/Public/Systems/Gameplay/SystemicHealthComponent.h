// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Systems/SystemicGameplayTags.h"

#include "SystemicGameplayComponentBase.h"

#include "SystemicHealthComponent.generated.h"

// Declarations.
class AController;
class UDamageType;

// Delegate for broadcasting health changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FSystemicHealthChangedSignature, float, HealthPrevious, float, HealthNew, float, HealthDelta, AActor*, InstigatorActor, UObject*, Source);
// Delegate for broadcasting any change in lifecycle state for a systemic object.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSystemicLifecycleStateChangedSignature, const FGameplayTag, LifecycleStateTag, AActor*, InstigatorActor, UObject*, Source);
// Delegate for broadcasting object state changes due to health changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSystemicLifecycleStateSignature, AActor*, InstigatorActor, UObject*, Source);

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
	// Maximum health value used when clamping current health.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName="Max Health", Category="Health|Config", meta=(ClampMin="0.0"))
	float HealthMax = 100.0f;
	
	// Current health value.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Health|Transient", meta=(ClampMin="0.0"))
	float Health = 0.0f;
	
	// Whether this component destroys its owner when health reaches zero (default: false).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Health|Config")
	uint8 bDestroyOwnerOnKilled : 1 = false;

	// Whether this component supports a downed state before death (default: false).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Health|Config")
	uint8 bDownedStateSupported : 1 = false;

	// Whether to bind to engine damage events (default: true).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Health|Config")
	uint8 bBindToDamageEvents : 1 = true;

protected:
	/**
	 *	Handle an engine damage event.
	 *		NOTE (trent, 5/4/26): Damage is negated in this method and passed along as a health modifier.
	 *	@param Actor Target actor for health modification.
	 *	@param Damage Health delta applied.
	 *	@param DamageType Damage type object (if relevant).
	 *	@param InstigatorActor Controller responsible for the damage (if relevant).
	 *	@param DamageCauser Actor that caused the damage.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	virtual void HandleTakeDamage(AActor* Actor, const float Damage, const UDamageType* DamageType, AController* InstigatorActor, AActor* DamageCauser);

	/**
	 *	Emit a systemic health change event.
	 *	@param HealthNew New health value after modification.
	 *	@param HealthPrevious Previous health value before modification.
	 *	@param HealthDelta Health delta applied.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param Source Object responsible for the change.
	 *	@returns True if the event was successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	virtual bool EmitHealthEvent(const float HealthNew, const float HealthPrevious, const float HealthDelta, AActor* InstigatorActor, UObject* Source);

	/**
	 *	Emit a systemic max health change event.
	 *	@param HealthMaxNew New max health value after modification.
	 *	@param HealthMaxPrevious Previous max health value before modification.
	 *	@param HealthMaxDelta Max health delta applied.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param Source Object responsible for the change.
	 *	@returns True if the event was successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	virtual bool EmitHealthMaxEvent(const float HealthMaxNew, const float HealthMaxPrevious, const float HealthMaxDelta, AActor* InstigatorActor, UObject* Source);

	/**
	 *	Emit a systemic lifecycle event.
	 *	@param EventTag Gameplay tag identifying the lifecycle event.
	 *	@param InstigatorActor Actor responsible for the state change.
	 *	@param Source Object responsible for the state change.
	 *	@returns True if the event was successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health", meta=(GameplayTagFilter=TAG_System_Event_Lifecycle))
	virtual bool EmitLifecycleEvent(const FGameplayTag& EventTag, AActor* InstigatorActor, UObject* Source);

public:
	// Broadcast when this component's health changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicHealthChangedSignature OnHealthChanged;

	// Broadcast when this component's max health changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicHealthChangedSignature OnHealthMaxChanged;

	// Broadcast when this component's lifecycle state changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifecycleStateChangedSignature OnLifecycleChanged;

	// Broadcast when this component is spawned.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifecycleStateSignature OnSpawned;

	// Broadcast when this component crosses into a downed state.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifecycleStateSignature OnDowned;

	// Broadcast when this component owner is revived.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifecycleStateSignature OnRevived;

	// Broadcast when health is zero and the owner is already downed (if it's a supported state).
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicLifecycleStateSignature OnKilled;
	
	/**
	 *	Set this component's health.
	 *	@param HealthIn New health value.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param Source Object responsible for the change.
	 *	@returns The current health value after modification.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	float SetHealth(const float HealthIn, AActor* InstigatorActor = nullptr, UObject* Source = nullptr);

	/**
	 *	Get this component's current health.
	 *	@returns Current health value.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Health")
	float GetHealth() const { return Health; }

	/**
	 *	Set this component's max health.
	 *		NOTE (trent, 5/4/26): Treating this as a first-class modifier and event is likely unnecessary, but it's here for the sake of the unknown.
	 *	@param HealthMaxIn New max health value.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param Source Object responsible for the change.
	 *	@returns The current max health value after modification.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Set Max Health", Category="Game|Systems|Health")
	float SetHealthMax(float HealthMaxIn, AActor* InstigatorActor = nullptr, UObject* Source = nullptr);

	/**
	 *	Get this component's maximum health.
	 *	@returns Maximum health value.
	 */
	UFUNCTION(BlueprintPure, DisplayName="Get Max Health", Category="Game|Systems|Health")
	float GetHealthMax() const { return HealthMax; }
	
	/**
	 *	Modify the current health of this component.
	 *	@param HealthDelta Health delta to apply; negative values damage, positive values heal.
	 *	@param InstigatorActor Actor responsible for the change.
	 *	@param Source Object responsible for the change.
	 *	@returns Modified health value.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Health")
	virtual float ModifyHealth(float HealthDelta, AActor* InstigatorActor = nullptr, UObject* Source = nullptr);
	
	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~UActorComponent.
};