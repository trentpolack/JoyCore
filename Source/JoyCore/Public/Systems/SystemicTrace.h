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
USTRUCT(BlueprintType)
struct JOYCORE_API FSystemicTraceEvaluatedConditionResult
{
	GENERATED_BODY()
	
	/**
	 *	FSystemicTraceEvaluatedConditionResults.
	 */
	
	/**
	 *	Pointer to the evaluated condition.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	TWeakObjectPtr<USystemicCondition> Condition = nullptr;
	
	/**
	 *	Output of the evaluation.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	bool bResult = true;
	
	/**
	 *	Text representation of the evaluation result.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	TArray<FString> EvaluationLog;
	
public:
	explicit FSystemicTraceEvaluatedConditionResult(const TWeakObjectPtr<USystemicCondition> ConditionIn = nullptr, const bool bResultIn = true, const FString& LogLine = FString())
	: Condition(ConditionIn)
	, bResult(bResultIn)
	, EvaluationLog({LogLine})
	{	}
};

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
	FGameplayTag EventTag = FGameplayTag();
	
	/**
	 *	Name of the rule being evaluated.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	FName RuleName = NAME_None;
	
	/**
	 *	List of condition evaluation results during the trace.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Systemic Trace")
	TArray<FSystemicTraceEvaluatedConditionResult> EvaluatedConditionResults;
	
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicTrace()
	{	}
};
