// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Conditions/SystemicCondition.h"

#include "Systems/Events/SystemicEvent.h"

// Base condition evaluation; so long as it's enabled and the data is valid it'll pass.
bool USystemicCondition::Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const
{
	return(bEnabled && ((Subject == ESystemicEventSubject::Instigator) && Event.Instigator.IsValid())
		|| (Subject == ESystemicEventSubject::Target && Event.Target.IsValid())
		|| (Subject == ESystemicEventSubject::SourceObject && Event.SourceObject.IsValid()));
}