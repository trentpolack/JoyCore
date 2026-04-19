// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Engine/DataAsset.h"

#include "Systems/SystemicGameplayTags.h"

#include "SystemicRule.generated.h"

// Declarations.
class USystemicCondition;
class USystemicReaction;

/**
 *	USystemicRule Class Definition.
 */
UCLASS(Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicRule : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	// Name of this rule, ideally unique.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rule")
	FName Name = NAME_None;

	// Gameplay tags of events that can trigger a reaction.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rule", meta=(GameplayTagFilter="System.Event"))
	FGameplayTagContainer TriggerEventTags = FGameplayTagContainer();

	// List of conditions that must all pass to trigger a reaction.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "Rule")
	TArray<TObjectPtr<USystemicCondition>> ConditionList;

	// List of reactions to trigger if all conditions are satisfied.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "Rule")
	TArray<TObjectPtr<USystemicReaction>> ReactionList;

	// Priority tag for this rule.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rule", meta=(GameplayTagFilter="System.Rule.Priority"))
	FGameplayTag Priority = TAG_System_Rule_Priority_Default;

	// Cooldown before this rule can be successfully evaluated again; defaults to 0.0f.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rule")
	float Cooldown = 0.0f;

	// Whether this rule is enabled or not; defaults to true.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rule")
	bool bEnabled = true;

public:
	/**
	 * Rule name accessor.
	 * @return Name of this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	const FName& GetRuleName() const;

	/**
	 * Get a const reference to the container of triggering event tags.
	 * @return Container of triggering event tags.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems", meta=(GameplayTagFilter="System.Event"))
	const FGameplayTagContainer& GetTriggerEventTags() const;

	/**
	 * Get a const reference to the list of conditions for this rule.
	 * @return List of conditions for this rule.
	 */
	const TArray<TObjectPtr<USystemicCondition>>& GetConditionList() const;

	/**
	 * Get a const reference to the list of reactions for this rule.
	 * @return List of reactions for this rule.
	 */
	const TArray<TObjectPtr<USystemicReaction>>& GetReactionList() const;

	/**
	 * Get the priority tag for this rule.
	 * @return Priority tag for this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	const FGameplayTag& GetPriority() const;
	
	/**
	 * Get the cooldown for this rule.
	 * @return Cooldown for this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	float GetCooldown() const;

	/**
	 * Get the state of this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	bool IsEnabled() const;

	/**
	 * Set the state of this rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	void Enable(bool bEnabledIn = true);
	
	/**
	 *	FSystemEvent Constructor. 
	 */
	USystemicRule()
	{	}
};
