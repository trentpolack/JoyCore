// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "SystemicRuleContext.generated.h"

/**
 *	FSystemicRuleContext Structure Definition.
 */
USTRUCT(BlueprintType)
struct JOYCORE_API FSystemicRuleContext
{
	GENERATED_BODY()
	
	/**
	 *	FSystemicRuleContext. 
	 */

	/**
	 *	Initiator of the event.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<AActor> Instigator = nullptr;
	/**
	 *	Target of the event, if there is one.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<AActor> Target = nullptr;
	/**
	 *	Source object that caused the event (for general-purpose coverage beyond actors). 
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<UObject> SourceObject = nullptr;

	/**
	 *	Cache of named objects throughout the evaluation of a ruleset and reaction. 
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TMap<FName, UObject*> NamedObjects;

	/**
	 *	Cache of named parameters throughout the evaluation of a ruleset and reaction. 
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TMap<FName, float> NamedValues;

	/**
	 *	FSystemicRuleContext Constructor. 
	 */
	FSystemicRuleContext()
	{	}
};
