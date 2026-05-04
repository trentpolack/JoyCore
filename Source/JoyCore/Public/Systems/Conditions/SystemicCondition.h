// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimTrace.h"

#include "GameFramework/Actor.h"
#include "UObject/Object.h"

#include "Systems/Traits/ISystemicTraitProvider.h"
#include "Systems/Events/SystemicEvent.h"
#include "Systems/SystemicTrace.h"

#include "SystemicCondition.generated.h"

// Declarations.
struct FSystemicRuleContext;

/**
 *	USystemicCondition Class Definition.
 *		Object that contains a condition which can take in a systemic event and evaluate it.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicCondition : public UObject
{
	GENERATED_BODY()

protected:
	// Name of this Condition; use for identification and organization. It does not affect functionality.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition|Config")
	FName Name = NAME_None;

	// Description of what this Condition is doing; useful for in-line documentation. It does not affect functionality.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition|Config")
	FText Description = FText();
	
	// Subject that this condition is evaluating against.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition|Config")
	ESystemicEventSubject Subject = ESystemicEventSubject::SourceObject;

	// Whether this Condition is enabled and should be evaluated (default: true).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition|Config")
	bool bEnabled = true;

public:
	/**
	 *	Get the trait provider interface for the subject of the given event.
	 *	@param Event Triggering event.
	 *	@param Trace Trace information for debugging and logging.
	 *	@return Trait provider for the subject; return nullptr if the subject does not implement ISystemicTraitProvider.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Conditions")
	TScriptInterface<ISystemicTraitProvider> GetSubjectTraitProvider(const FSystemicEvent& Event, FSystemicTrace& Trace) const;
	
	/**
	 * Evaluate this condition given the triggering event, cached data in the rule context, and built-up trace information. The base implementation will do a check to ensure the condition is enabled and the subject is valid (so it works as a basic null check).
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Conditions")
	virtual bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const;
};
