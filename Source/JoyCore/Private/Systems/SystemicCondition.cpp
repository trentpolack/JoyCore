// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicCondition.h"

#include "Systems/SystemicEvent.h"

// Base condition evaluation; so long as the data is valid it'll pass.
bool USystemicCondition::Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const
{
	return(bEnabled && (Event.Instigator.IsValid() || Event.Target.IsValid() || Event.SourceObject.IsValid()));
}