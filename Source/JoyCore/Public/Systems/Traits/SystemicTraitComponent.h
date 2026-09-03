// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTags.h"

#include "Systems/Traits/ISystemicTraitProvider.h"

#include "SystemicTraitComponent.generated.h"

// Delegate for broadcasting when a trait provider's traits have changed; contains the new current trait tags, container of only new tags, and container of only removed tags.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSystemicTraitSignature, UObject*, Object, const FGameplayTagContainer&, TraitTags, const FGameplayTagContainer&, TraitTagsNew, const FGameplayTagContainer&, TraitTagsRemoved);

/**
 *	USystemicTraitComponent Class.
 *		Actor component that implements all the base data and functionality for ISystemicTraitProvider, provides OnTraitChanged/OnStateChanged delegates, and emits relevant events to the systemic world.
 */
UCLASS(Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicTraitComponent : public UActorComponent, public ISystemicTraitProvider
{
	GENERATED_BODY()

public:
	USystemicTraitComponent();
	
protected:
	// Trait Tags at startup.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Traits|Config", meta=(GameplayTagFilter="System.Trait", DisplayName="Initial Traits", DisplayPriority="1"))
	FGameplayTagContainer TraitsInitial = FGameplayTagContainer();
	
	// Trait Tag Container (runtime).
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Traits|Transient", meta=(GameplayTagFilter="System.Trait"))
	FGameplayTagContainer Traits = FGameplayTagContainer();
	
	// Whether to emit object-level lifecycle events like creation/destruction (default: true).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Traits|Config")
	uint8 bEmitLifecycleEvents : 1 = true;

protected:
	/**
	 *	Emit an object lifecycle event; namely, creation and destruction (_not_ health-related gameplay lifecycle).
	 *	@param EventTag Tag to emit.
	 *	@returns True if the event was successfully emitted.
	 */
	UFUNCTION(Category="Game|Systems|Events", meta=(GameplayTagFilter="System.Event"))
	virtual bool EmitLifecycleEvent(const FGameplayTag& EventTag);

	/**
	 *	Broadcast OnTraitsChanged and, for state changes, also broadcast OnStateChanged.
	 *	@param Object Object that is being changed.
	 *	@param TraitTags Current trait tags.
	 *	@param TraitTagsNew New trait tags.
	 *	@param TraitTagsRemoved Removed trait tags.
	 */
	bool BroadcastEvents(UObject* Object, const FGameplayTagContainer& TraitTags, const FGameplayTagContainer& TraitTagsNew, const FGameplayTagContainer& TraitTagsRemoved);

	// ISystemicTraitComponent (protected).
	virtual const FName GetOwnerName() const override;
	// ~ISystemicTraitComponent (protected).

public:
	// Broadcast when this component's Traits are changed (including State changes).
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicTraitSignature OnTraitsChanged;

	// Broadcast when this component's Traits are changed (excludes non-state changes).
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicTraitSignature OnStateChanged;

	// ISystemicTraitProvider.
	virtual bool AddTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent = true) override;
	virtual const FGameplayTagContainer AddTraits(const FGameplayTagContainer& TraitTags, bool bEmitTraitChangedEvent = true) override;
	virtual bool RemoveTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent = true) override;
	virtual const FGameplayTagContainer RemoveTraits(const FGameplayTagContainer& TraitTags, bool bEmitTraitChangedEvent = true) override;
	virtual bool ModifyTraits(const FGameplayTagContainer& TraitTagsToAdd, const FGameplayTagContainer& TraitTagsToRemove) override;
	
	virtual const FGameplayTagContainer& GetTraits() const override;
	// ~ISystemicTraitComponent.
	
	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~UActorComponent.
};
