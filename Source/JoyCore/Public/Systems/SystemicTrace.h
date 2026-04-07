// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "SystemicTrace.generated.h"

// Declarations.
class USystemicCondition;

/**
 *	FSystemicTraceEvaluatedConditionResults Structure definition.
 */
USTRUCT(BlueprintType, Category="Game|Systems")
struct JOYCORE_API FSystemicTraceEvaluatedConditionResult
{
	GENERATED_BODY()
	
	// Pointer to the evaluated condition.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	TWeakObjectPtr<USystemicCondition> Condition = nullptr;
	
	// Output of the evaluation.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	bool bResult = true;
	
	// Text representation of the evaluation result.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	TArray<FString> EvaluationLog;
	
public:
	/**
	 * FSystemicTraceEvaluatedConditionResult Constructor.
	 * @param ConditionIn Evaluated Condition.
	 * @param bResultIn The result of the evaluation.
	 * @param LogLineIn Log message from the evaluation.
	 */
	explicit FSystemicTraceEvaluatedConditionResult(const TWeakObjectPtr<USystemicCondition> ConditionIn = nullptr, const bool bResultIn = true, const FString& LogLineIn = FString())
	: Condition(ConditionIn)
	, bResult(bResultIn)
	, EvaluationLog({LogLineIn})
	{	}
};

/**
 *	FSystemicTrace Structure Definition.
 */
USTRUCT(BlueprintType, Category="Game|Systems")
struct JOYCORE_API FSystemicTrace
{
	GENERATED_BODY()
	
	// Event tag.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	FGameplayTag EventTag = FGameplayTag();
	
	// Name of the rule being evaluated.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	TArray<FName> RuleNames;
	
	// List of condition evaluation results during the trace.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Systems|Trace")
	TArray<FSystemicTraceEvaluatedConditionResult> EvaluatedConditionResults;
	
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicTrace()
	{	}
};
