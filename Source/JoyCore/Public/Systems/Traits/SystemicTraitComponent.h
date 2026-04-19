// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTags.h"

#include "Systems/Traits/ISystemicTraitProvider.h"

#include "SystemicTraitComponent.generated.h"

UCLASS(Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicTraitComponent : public UActorComponent, public ISystemicTraitProvider
{
	GENERATED_BODY()

public:
	USystemicTraitComponent();
	
protected:
	// Trait Tag Container.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Traits")
	FGameplayTagContainer TraitTags = FGameplayTagContainer();

public:
	// ISystemicTraitProvider.
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual void AddTraitTag(const FGameplayTag& Tag) override;
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual void AddTraitTags(const FGameplayTagContainer& TagContainer) override;
	virtual const FGameplayTagContainer& GetTraitTags() const override;
	// ~ISystemicTraitComponent.
	
	// UActorComponent.
	virtual void BeginPlay() override;
	// ~UActorComponent.
};
