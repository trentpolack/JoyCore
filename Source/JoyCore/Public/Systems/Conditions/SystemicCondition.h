// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "SystemicTrace.h"
#include "Animation/AnimTrace.h"

#include "UObject/Object.h"

#include "Systems/ISystemicTraitProvider.h"
#include "Systems/SystemicEvent.h"

#include "SystemicCondition.generated.h"

// Declarations.
struct FSystemicRuleContext;
struct FSystemicTrace;

/**
 *	Object that contains a condition which can take in a systemic event and evaluate it.
 */
UCLASS(EditInlineNew, BlueprintType, ClassGroup=(JoyCore), meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicCondition : public UObject
{
	GENERATED_BODY()

protected:
	/**
	 *	USystemicCondition. 
	 */

	/**
	 *	Name of this Condition; use for identification and organization. It does not affect functionality.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FName Name = NAME_None;

	/**
	 *	Description of what this Condition is doing; useful for in-line documentation. It does not affect functionality.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FText Description = FText();
	
	/**
	 *	Subject that this condition is evaluating against.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	ESystemicEventSubject Subject = ESystemicEventSubject::SourceObject;

	/**
	 *	Whether this Condition is enabled and should be evaluated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	bool bEnabled = true;

public:
	/**
	 *	Get the trait provider interface for the subject of the given event.
	 *	@param Event Triggering event.
	 *	@param Trace Trace information for debugging and logging.
	 *	@return Trait provider for the subject; return nullptr if the subject does not implement ISystemicTraitProvider.
	 */
	UFUNCTION(BlueprintCallable, Category="Conditions")
	const ISystemicTraitProvider* GetSubjectTraitProvider(const FSystemicEvent& Event, FSystemicTrace& Trace) const
	{
		const ISystemicTraitProvider* pSubjectTraitProvider = Cast<ISystemicTraitProvider>(Event.GetObjectBySubject(Subject));
		if(!pSubjectTraitProvider)
		{
			// Subject does not implement ISystemicTraitProvider. Log this in the trace and return nullptr.
			Trace.EvaluatedConditionResults.Add(FSystemicTraceEvaluatedConditionResult(const_cast<USystemicCondition*>(this), false, TEXT("Subject does not implement ISystemicTraitProvider.")));
			return nullptr;
		}
		
		return pSubjectTraitProvider;
	}
	
	/**
	 * Evaluate this condition given the triggering event, cached data in the rule context, and built-up trace information.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Conditions")
	virtual bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const;
};