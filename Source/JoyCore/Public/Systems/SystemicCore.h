// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "SystemicCore.generated.h"

// Declarations.
class USystemicRule;

struct FSystemicEvent;
struct FSystemicTrace;

/**
 * USystemicCore Class Definition.
 *	Core config settings, helpers, and utilities. Not intended to be instanced.
 */
UCLASS(NotBlueprintable, NotBlueprintType, Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicCore : public UObject
{
	GENERATED_BODY()

public:
	/**
	 *	Events.
	 */
	
	/**
	 * Get which of the two passed-in Events is higher-priority.
	 * @param EventA First Event to compare
	 * @param EventB Second Event to compare
	 * @return The Event with higher priority; EventA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	static const FSystemicEvent& GetHigherPriorityEvent(const FSystemicEvent& EventA, const FSystemicEvent& EventB);
	
	
	
	/**
	 *	Rules.
	 */

	/**
	 * Get which of the two passed-in rules is higher-priority.
	 * @param RuleA First rule to compare
	 * @param RuleB Second rule to compare
	 * @return The rule with higher priority; RuleA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	static const USystemicRule* GetHigherPriorityRule(const USystemicRule* RuleA, const USystemicRule* RuleB);
};