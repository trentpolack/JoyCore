// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "SystemicCondition.generated.h"

// Declarations.
struct FSystemicEvent;
struct FSystemicRuleContext;
struct FSystemicTrace;

/**
 *	Object which contains a condition which can take in a systemic event and evaluate it.
 */
UCLASS(EditInlineNew, ClassGroup=(JoyCore), meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicCondition : public UObject
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicCondition. 
	 */

	/**
	 * Evaluate this condition given the triggering event, cached data in the rule context, and built up trace information.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the condition is met, false otherwise.
	 */
	bool Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace);
};