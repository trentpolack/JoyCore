// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "SystemicRule.generated.h"

/**
 *	FSystemRule Structure Definition.
 */
USTRUCT(BlueprintType, ClassGroup=(JoyCore))
struct JOYCORE_API FSystemicRule
{
	GENERATED_BODY()
	
	/**
	 *	FSystemRule. 
	 */

	/**
	 *	Name of this rule, ideally unique.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	FName Name = NAME_None;

	/**
	 *	Gameplay tags of events that can trigger a reaction.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	FGameplayTagContainer TriggerEventTags;

	/**
	 *	List of conditions that must all pass to trigger a reaction.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	TArray<TMap<ESystemicSubject, FGameplayTagQuery>> ConditionList;
	
	/**
	 *	Priority tag for this rule.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	FGameplayTag Priority;

	/**
	 *	Cooldown before this rule can be successfully evaluated again; defaults to 0.0f.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	float Cooldown = 0.0f;

	/**
	 *	Whether this rule is enabled or not; defaults to true.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rule")
	bool Enabled = true;

	/**
	 *	FSystemEvent Constructor. 
	 */
	FSystemicRule()
	{	}
};
