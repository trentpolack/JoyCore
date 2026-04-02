// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "SystemicTrace.generated.h"

/**
 *	FSystemicTrace Structure Definition.
 */
USTRUCT(BlueprintType)
struct JOYCORE_API FSystemicTrace
{
	GENERATED_BODY()
	
	/**
	 *	FSystemicTrace. 
	 */
	
	/**
	 *	Event tag.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	FGameplayTag EventTag;
	
	/**
	 *	Name of the rule being evaluated.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	FName RuleName;
	
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicTrace()
	{	}
};
