// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "SystemicGameplayComponentBase.h"

#include "GameplayTags.h"

#include "SystemicContactComponent.generated.h"

// Declarations.
class UPrimitiveComponent;

// Contact event delegate broadcast when the collision component is hit.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSystemicContactHitSignature, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComponent, const FHitResult&, HitResult);
// Contact event delegate broadcast when the collision component begins overlapping another component.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FSystemicContactOverlapBeginSignature, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComponent, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);
// Contact event delegate broadcast when the collision component ends overlapping another component.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSystemicContactOverlapEndSignature, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComponent, int32, OtherBodyIndex);

/**
 *	USystemicContactComponent Class.
 *		Actor component that implements hit and overlap logic and emits appropriate events.
 */
UCLASS(Category="Game|Systems|Components", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicContactComponent : public USystemicGameplayComponentBase
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicContactComponent Constructor.
	 */
	USystemicContactComponent();

protected:
	// Whether this component emits systemic contact events for hit notifications (default: true).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Contact|Config")
	uint8 bEmitHitEvents : 1 = true;

	// Whether this component emits systemic contact events for begin/end overlap notifications (default: true).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Contact|Config")
	uint8 bEmitOverlapEvents : 1 = true;

	// Primitive component whose hit and overlap events are observed; if unset, this component uses its owner's.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Contact|Config")
	TObjectPtr<UPrimitiveComponent> CollisionComponent = nullptr;
	
	// Minimum hit impulse magnitude required before a hit event is emitted (default: 0.0).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Contact|Config", meta=(ClampMin="0.0"))
	float HitImpulseThreshold = 0.0f;

	// Whether this component emits systemic contact events for hit notifications (default: true).
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Contact|Transient")
	uint8 bCollisionEventsBound : 1 = false;

protected:
	/**
	 *	Emit a systemic contact event.
	 *	@param EventTag Gameplay tag identifying the contact event.
	 *	@param OtherActor Actor involved in the contact.
	 *	@param Source Object responsible for the contact event.
	 *	@param HitResult Hit result structure filled out from the contact event.
	 *	@param Magnitude General-purpose magnitude/value associated with the contact event.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact", meta=(GameplayTagFilter="System.Event"))
	virtual bool EmitContactEvent(const FGameplayTag& EventTag, AActor* OtherActor, UObject* Source, const FHitResult& HitResult, float Magnitude);

	/**
	 *	Handle a hit notification from the observed primitive component.
	 *	@param HitComponent Component that received the hit.
	 *	@param OtherActor Actor that hit or was hit by this component.
	 *	@param OtherComponent The other component involved in the hit.
	 *	@param NormalImpulse Normal impulse applied by the hit.
	 *	@param HitResult Hit result describing the contact.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact")
	virtual void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult);

	/**
	 *	Handle a begin-overlap notification from the observed primitive component.
	 *	@param OverlappedComponent Component that began overlapping.
	 *	@param OtherActor Actor that began overlapping this component.
	 *	@param OtherComponent Other component involved in the overlap.
	 *	@param OtherBodyIndex Body index of the other component.
	 *	@param bFromSweep True if the overlap came from a sweep.
	 *	@param SweepResult Sweep result associated with the overlap, if applicable.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact")
	virtual void HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 *	Handle an end-overlap notification from the observed primitive component.
	 *	@param OverlappedComponent Component that ended overlapping.
	 *	@param OtherActor Actor that stopped overlapping this component.
	 *	@param OtherComponent Other component involved in the overlap.
	 *	@param OtherBodyIndex Body index of the other component.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact")
	virtual void HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
public:
	// Broadcast when this component's temperature changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicContactHitSignature OnHit;

	// Broadcast when this component's temperature changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicContactOverlapBeginSignature OnOverlapBegin;

	// Broadcast when this component's temperature changes.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicContactOverlapEndSignature OnOverlapEnd;
	
	/**
	 *	Set the primitive component to loosely bind to this component.
	 *	@param Component Primitive component to observe.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact")
	void SetCollisionComponent(UPrimitiveComponent* Component);

	/**
	 *	Get the primitive component loosely bound to this component.
	 *	@returns Primitive component being listened to.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Contact")
	UPrimitiveComponent* GetCollisionComponent() const;

	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~UActorComponent.
};