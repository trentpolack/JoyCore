// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTags.h"

#include "ISystemicTraitProvider.h"

#include "SystemicTraitComponent.generated.h"

UCLASS(ClassGroup=(JoyCore), meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicTraitComponent : public UActorComponent, public ISystemicTraitProvider
{
	GENERATED_BODY()

public:
	USystemicTraitComponent();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Traits")
	FGameplayTagContainer TraitTags;

public:
	/**
	 *	USystemicTraitComponent.
	 */
	
	/**
	 *	Adds a trait tag to the component.
	 *	@param Tag The tag to add.
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual void AddTraitTag(const FGameplayTag& Tag);
	/**
	 *	Adds trait tags to the component.
	 *	@param TagContainer Container of tags to add.
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual void AddTraitTags(const FGameplayTagContainer& TagContainer);

	/**
	 *	ISystemicTraitProvider.
	 */
	virtual const FGameplayTagContainer& GetTraitTags() const override;
	
protected:
	virtual void BeginPlay() override;
};
