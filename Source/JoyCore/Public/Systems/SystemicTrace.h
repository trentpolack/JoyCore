// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Templates/Tuple.h"
#include "Templates/TupleFwd.h"
#include "CoreTypes.h"
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
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "Systems|Trace")
	FGameplayTag EventTag = FGameplayTag();
	
	// List of condition evaluation results during the trace.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, AdvancedDisplay, Category = "Systems|Trace")
	TArray<FSystemicTraceEvaluatedConditionResult> EvaluatedConditionResults;
	
	// List of names and results of evaluated rules.
	TArray<TPair<FName, bool>> RuleNameAndResultList;

	// List of names and results in the executed rule's reactions.
	TArray<TPair<FName, bool>> RuleReactionNameAndResultList;

	/**
	 *	Get the evaluation log as a string.
	 */
	FString GetEvaluationLogAsString() const
	{
		FString log;
		
		log+= FString::Printf(TEXT("Event Tag: %s\n"), *EventTag.ToString());
		log+= FString::Printf(TEXT("Rule Name(s): %s\n"), *FString::JoinBy(RuleNameAndResultList, TEXT(", "), [](const TPair<FName, bool>& Name){ return FString::Printf(TEXT("%s: %s"), *Name.Key.ToString(), Name.Value ? TEXT("Passed") : TEXT("Failed")); }));
		
		// Go through all the conditional results.
		for(const FSystemicTraceEvaluatedConditionResult& result : EvaluatedConditionResults)
		{
			for(const FString& line : result.EvaluationLog)
			{
				// Print out the evaluation log.
				log+= FString::Printf(TEXT("\t%s\n"), *line);
			}
		}
		
		log+= FString::Printf(TEXT("Rule Reaction Execution Results:\n"));
		log+= FString::Printf(TEXT("\t%s\n"), *FString::JoinBy(RuleReactionNameAndResultList, TEXT(", "), [](const TPair<FName, bool>& Name){ return FString::Printf(TEXT("%s: %s"), *Name.Key.ToString(), Name.Value ? TEXT("Succeeded") : TEXT("Failed")); }));

		return log;
	}
	
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicTrace()
	{	}
};
