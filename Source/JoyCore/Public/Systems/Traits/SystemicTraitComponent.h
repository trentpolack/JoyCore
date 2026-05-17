// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTags.h"

#include "Systems/Traits/ISystemicTraitProvider.h"

#include "SystemicTraitComponent.generated.h"

// Delegate for broadcasting when a trait provider's traits have changed; contains the new current trait tags as well as a container with only the new tags.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSystemicTraitSignature, UObject*, Object, const FGameplayTagContainer&, TraitTags, const FGameplayTagContainer&, TraitTagsNew);

UCLASS(Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicTraitComponent : public UActorComponent, public ISystemicTraitProvider
{
	GENERATED_BODY()

public:
	USystemicTraitComponent();
	
protected:
	// Trait Tag Container.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Traits|Transient", meta=(GameplayTagFilter="System.Trait"))
	FGameplayTagContainer Traits = FGameplayTagContainer();

	// Whether to emit object-level lifecycle events like creation/destruction (default: true).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Traits|Config")
	uint8 bEmitLifecycleEvents : 1 = true;

protected:
	/**
	 *	Emit a lifecycle event; namely, creation and destruction.
	 *	@param EventTag Tag to emit.
	 *	@return True if the event was successfully emitted.
	 */
	virtual bool EmitLifecycleEvent(const FGameplayTag& EventTag);

public:
	// Broadcast when this component successfully receives an interaction.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Traits|Events")
	FSystemicTraitSignature OnTraitsChanged;
	
	// ISystemicTraitProvider.
	virtual bool AddTrait(const FGameplayTag& TraitTag) override;
	virtual bool AddTraits(const FGameplayTagContainer& TraitTagContainer) override;
	virtual bool RemoveTrait(const FGameplayTag& TraitTag) override;
	virtual bool RemoveTraits(const FGameplayTagContainer& TraitTagContainer) override;
	virtual const FGameplayTagContainer& GetTraits() const override;
	// ~ISystemicTraitComponent.
	
	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~UActorComponent.
};
