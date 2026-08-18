// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Engine/DataAsset.h"

#include "JoyCoreNativeGameplayTags.h"

#include "SystemicRule.generated.h"

// Declarations.
class USystemicCondition;
class USystemicReaction;

/**
 *	USystemicRule Class Definition.
 *		Base definition for a rule that governs system behaviors/reactions based on events and conditions.
 */
UCLASS(Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicRule : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	// Name of this rule, ideally unique.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta = (DisplayPriority="1"))
	FName RuleName = NAME_None;

	// Description of this rule, providing context and purpose.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta = (DisplayAfter="RuleName"))
	FString RuleDescription = TEXT("");

	// Gameplay tags of events that can trigger a reaction.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta = (GameplayTagFilter="System.Event"))
	FGameplayTagContainer TriggerEventTags = FGameplayTagContainer();

	// List of conditions that must all pass to trigger a reaction.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "SystemicRule")
	TArray<TObjectPtr<USystemicCondition>> ConditionList;

	// List of reactions to trigger if all conditions are satisfied.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "SystemicRule")
	TArray<TObjectPtr<USystemicReaction>> ReactionList;

	// Priority tag for this rule.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta = (GameplayTagFilter="System.Rule.Priority", DisplayAfter="RuleDescription"))
	FGameplayTag Priority = TAG_System_Rule_Priority_Default;

	// Cooldown before this rule can be successfully evaluated again; defaults to 0.0f.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta=(DisplayAfter="Priority"))
	float Cooldown = 0.0f;

	// Whether this rule is enabled; defaults to true.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SystemicRule", meta=(DisplayAfter="Cooldown"))
	uint8 bEnabled : 1 = true;

public:
	/**
	 * Rule Name accessor.
	 * @returns Name of this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	const FName& GetRuleName() const;

	/**
	 * Rule Description accessor.
	 * @returns Description for this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	const FString& GetRuleDescription() const;

	/**
	 * Get a const reference to the container of triggering event tags.
	 * @returns Container of triggering event tags.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules", meta = (GameplayTagFilter="System.Event"))
	const FGameplayTagContainer& GetTriggerEventTags() const;

	/**
	 * Get a const reference to the list of conditions for this rule.
	 * @returns List of conditions for this rule.
	 */
	const TArray<TObjectPtr<USystemicCondition>>& GetConditionList() const;

	/**
	 * Get a const reference to the list of reactions for this rule.
	 * @returns List of reactions for this rule.
	 */
	const TArray<TObjectPtr<USystemicReaction>>& GetReactionList() const;

	/**
	 * Get the priority tag for this rule.
	 * @returns Priority tag for this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	const FGameplayTag& GetPriority() const;
	
	/**
	 * Get the cooldown for this rule.
	 * @returns Cooldown for this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	float GetCooldown() const;

	/**
	 * Get the state of this rule.
	 * @returns Returns true if the rule is enabled.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	bool GetIsEnabled() const;

	/**
	 * Set the state of this rule.
	 * @param bEnabledIn New state for the rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Rules")
	bool Enable(bool bEnabledIn = true);
	
	/**
	 *	FSystemEvent Constructor. 
	 */
	USystemicRule()
	{	}
};
