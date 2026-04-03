// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Systems/Conditions/SystemicCondition.h"

#include "SystemicConditionHasTraitQuery.generated.h"

/**
 *	USystemicConditionHasTraitQuery Class Definition.
 *		This class evaluates an event subject with the ISystemicTraitProvider interface to determine if the subject matches a specific trait query.
 */
UCLASS()
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
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