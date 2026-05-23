// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReaction.h"

#include "Systems/Events/SystemicEvent.h"
#include "Systems/SystemicTrace.h"

// Accessor for the reaction name.
const FName& USystemicReaction::GetReactionName() const
{
	return ReactionName;
}

// Execute this reaction from the triggering event, cached data in the rule context, and built-up trace information. The base implementation will just log that it has been executed.
bool USystemicReaction::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(GetReactionName(), true));
	return true;
}