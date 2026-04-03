// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimTrace.h"

#include "UObject/Object.h"

#include "Systems/Traits/ISystemicTraitProvider.h"
#include "Systems/Events/SystemicEvent.h"
#include "Systems/SystemicTrace.h"

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
	TScriptInterface<ISystemicTraitProvider> GetSubjectTraitProvider(const FSystemicEvent& Event, FSystemicTrace& Trace) const
	{
		UObject* SubjectObject = Event.GetObjectBySubject(Subject).Get();
		ISystemicTraitProvider* SubjectTraitProvider = Cast<ISystemicTraitProvider>(SubjectObject);
		if(!SubjectTraitProvider)
		{
			// Subject does not implement ISystemicTraitProvider. Log this in the trace and return nullptr.
			Trace.EvaluatedConditionResults.Add(FSystemicTraceEvaluatedConditionResult(const_cast<USystemicCondition*>(this), false, TEXT("Subject does not implement ISystemicTraitProvider.")));
			return nullptr;
		}

		// Set both parts of the script interface.
		TScriptInterface<ISystemicTraitProvider> SubjectTraitProviderInterface;
		SubjectTraitProviderInterface.SetObject(SubjectObject);
		SubjectTraitProviderInterface.SetInterface(SubjectTraitProvider);
		return SubjectTraitProviderInterface;
	}
	
	/**
	 * Evaluate this condition given the triggering event, cached data in the rule context, and built-up trace information.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Conditions")
	virtual bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const;
};