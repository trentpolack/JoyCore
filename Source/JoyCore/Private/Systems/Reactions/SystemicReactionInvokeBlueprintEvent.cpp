// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionInvokeBlueprintEvent.h"

// Execute the blueprint-implemented event as a reaction to the triggering event.
bool USystemicReactionInvokeBlueprintEvent::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	// Execute the blueprint-implemented event.
	bool bSuccess = BP_OnReaction(Event, Context, Trace);
	
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, bSuccess));
	return bSuccess;
}