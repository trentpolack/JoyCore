// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Engine/DataAsset.h"

#include "SystemicRuleAsset.generated.h"

// Declarations.
class USystemicCondition;

/**
 *	USystemicRuleAsset Class Definition.
 */
UCLASS(ClassGroup=(JoyCore))
class JOYCORE_API USystemicRuleAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	/**
	 *	USystemicRuleAsset. 
	 */

	/**
	 *	Name of this rule, ideally unique.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
	FName Name = NAME_None;

	/**
	 *	Gameplay tags of events that can trigger a reaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Rule")
	FGameplayTagContainer TriggerEventTags = FGameplayTagContainer();

	/**
	 *	List of conditions that must all pass to trigger a reaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Rule")
	TArray<TObjectPtr<USystemicCondition>> ConditionList;
	
	/**
	 *	Priority tag for this rule.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
	FGameplayTag Priority = FGameplayTag();

	/**
	 *	Cooldown before this rule can be successfully evaluated again; defaults to 0.0f.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
	float Cooldown = 0.0f;

	/**
	 *	Whether this rule is enabled or not; defaults to true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
	bool bEnabled = true;
	
public:
	/**
	 *	FSystemEvent Constructor. 
	 */
	USystemicRuleAsset()
	{	}
};
