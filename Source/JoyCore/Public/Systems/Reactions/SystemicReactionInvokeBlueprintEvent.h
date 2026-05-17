// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Reactions/SystemicReaction.h"

#include "SystemicReactionInvokeBlueprintEvent.generated.h"

/**
 *	USystemicReactionInvokeBlueprintEvent Class Definition.
 *		This class reacts to an event by invoking a blueprint event.
 */
UCLASS()
class JOYCORE_API USystemicReactionInvokeBlueprintEvent : public USystemicReaction
{
	GENERATED_BODY()

public:
	/**
	 * Blueprint-implementable event for implementing reaction logic.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @return True if the execution is complete, false if there's an error along the way.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Game|Systems|Reactions", meta = (DisplayName = "On Reaction"))
	bool OnReaction(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace);

	// USystemicReaction.
	virtual bool Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) override;
	// ~USystemicReaction.
};