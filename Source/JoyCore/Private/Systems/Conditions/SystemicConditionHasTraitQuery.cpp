// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Conditions/SystemicConditionHasTraitQuery.h"

#include "Systems/ISystemicTraitProvider.h"
#include "Systems/SystemicTrace.h"

// Evaluate the subject's traits with a tag query.
bool USystemicConditionHasTraitQuery::Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const
{
	FSystemicTraceEvaluatedConditionResult result(const_cast<USystemicConditionHasTraitQuery*>(this));
	
	// The base method should always pass as it provides a basic condition validation.
	if(!Super::Evaluate(Event, Context, Trace))
	{
		result.bResult = false;
		result.EvaluationLog.Add(TEXT("Invalid condition data."));
		
		Trace.EvaluatedConditionResults.Add(result);
		return false;
	}
	
	// Get the trait provider interface.
	TScriptInterface<ISystemicTraitProvider> pSubjectTraitProvider = GetSubjectTraitProvider(Event, Trace);
	if(!pSubjectTraitProvider)
	{
		return false;
	}

	// Actually run the query now.
	if(pSubjectTraitProvider->QueryTraits(TraitQuery))
	{
		result.bResult = true;
		result.EvaluationLog.Add(TEXT("Condition trait query passed."));
	}
	else
	{
		result.bResult = false;
		result.EvaluationLog.Add(TEXT("Condition trait query failed."));
	}
	
	Trace.EvaluatedConditionResults.Add(result);
	return result.bResult;
}