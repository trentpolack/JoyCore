// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Systems/SystemicCondition.h"

#include "SystemicConditionHasTraitQuery.generated.h"

/**
 *	Object which contains a condition which can take in a systemic event and evaluate it.
 */
UCLASS(EditInlineNew, BlueprintType, ClassGroup=(JoyCore), meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicConditionHasTraitQuery : public USystemicCondition
{
	GENERATED_BODY()

protected:
	/**
	 *	USystemicConditionHasTraitQuery. 
	 */

	/**
	 *	Name of this Condition; use for identification and organization. It does not affect functionality.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FGameplayTagQuery TraitQuery;

	/**
	 *	~USystemicConditionHasTraitQuery. 
	 */

public:
	/**
	 *	USystemicCondition. 
	 */

	/**
	 * Evaluate this condition given the triggering event, cached data in the rule context, and built-up trace information.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the condition is met, false otherwise.
	 */
	virtual bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const override;

	/**
	 *	~USystemicCondition. 
	 */
};