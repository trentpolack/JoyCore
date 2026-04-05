// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Conditions/SystemicCondition.h"

#include "Systems/Events/SystemicEvent.h"

// Base condition evaluation; so long as it's enabled and the data is valid it'll pass.
bool USystemicCondition::Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const
{
	FSystemicTraceEvaluatedConditionResult result;
	result.Condition = this;
	result.bResult = bEnabled && (((Subject == ESystemicEventSubject::Instigator) && Event.Instigator.IsValid())
		|| (Subject == ESystemicEventSubject::Target && Event.Target.IsValid())
		|| (Subject == ESystemicEventSubject::SourceObject && Event.SourceObject.IsValid()));
	
	if(result.bResult)
	{
		result.EvaluationLog.Add(TEXT("Condition enabled and passed base validation test on subject object."));
	}
	else
	{
		result.EvaluationLog.Add(TEXT("Condition enabled and failed base validation test on subject object."));
	}

	Trace.EvaluatedConditionResults.Add(result);
	return(result.bResult);
}