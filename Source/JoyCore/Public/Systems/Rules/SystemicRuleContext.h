// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "SystemicRuleContext.generated.h"

/**
 *	FSystemicRuleContext Structure Definition.
 *		Used to accumulate context and information/results while processing a rule. Not intended for Blueprint usage.
 */
USTRUCT(BlueprintType, Category="Game|Systems")
struct JOYCORE_API FSystemicRuleContext
{
	GENERATED_BODY()
	
	// Initiator of the event.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "RuleContext")
	TWeakObjectPtr<AActor> Instigator = nullptr;
	// Target of the event, if there is one.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "RuleContext")
	TWeakObjectPtr<UObject> Target = nullptr;
	// Source object that caused the event (for general-purpose coverage beyond actors). 
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "RuleContext")
	TWeakObjectPtr<UObject> Source = nullptr;

	// Cache of named objects throughout the evaluation of a ruleset and reaction. 
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "RuleContext")
	TMap<FName, UObject*> NamedObjects;

	// Cache of named parameters throughout the evaluation of a ruleset and reaction. 
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "RuleContext")
	TMap<FName, float> NamedValues;

	/**
	 *	FSystemicRuleContext Constructor. 
	 */
	FSystemicRuleContext()
	{	}
};
