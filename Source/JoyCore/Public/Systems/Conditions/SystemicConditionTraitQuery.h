// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "Systems/Conditions/SystemicCondition.h"

#include "SystemicConditionTraitQuery.generated.h"

/**
 *	USystemicConditionTraitQuery Class Definition.
 *		This class evaluates an event subject with the ISystemicTraitProvider interface to determine if the subject matches a specific trait query.
 */
UCLASS()
class JOYCORE_API USystemicConditionTraitQuery : public USystemicCondition
{
	GENERATED_BODY()

protected:
	// Name of this Condition; use for identification and organization. It does not affect functionality.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition|Config", meta=(GameplayTagFilter="System.Trait"))
	FGameplayTagQuery TraitQuery;

public:
	/**
	 *	USystemicConditionTraitQuery Constructor.
	 */
	USystemicConditionTraitQuery();

	// USystemicCondition.	
	virtual bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const override;
	// ~USystemicCondition.
};