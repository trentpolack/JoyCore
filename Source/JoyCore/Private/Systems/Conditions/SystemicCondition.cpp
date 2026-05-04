// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Conditions/SystemicCondition.h"

#include "Systems/Events/SystemicEvent.h"

// Get the trait provider for the subject of the event, handling both actor and component-based implementations.
TScriptInterface<ISystemicTraitProvider> USystemicCondition::GetSubjectTraitProvider(const FSystemicEvent& Event, FSystemicTrace& Trace) const
{
	UObject* pSubjectObject = Event.GetObjectBySubject(Subject).Get();
	ISystemicTraitProvider* pSubjectTraitProvider = Cast<ISystemicTraitProvider>(pSubjectObject);
	if(!pSubjectTraitProvider)
	{
		// There was no trait provider on the object, attempt to find it on the actor.
		if(TObjectPtr<AActor> pSubjectActor = Cast<AActor>(pSubjectObject))
		{
			pSubjectObject = pSubjectActor->FindComponentByInterface(USystemicTraitProvider::StaticClass());
			pSubjectTraitProvider = Cast<ISystemicTraitProvider>(pSubjectObject);
		}
	}

	if(!pSubjectTraitProvider)
	{
		// Subject does not implement ISystemicTraitProvider. Log this in the trace and return nullptr.
		Trace.EvaluatedConditionResults.Add(FSystemicTraceEvaluatedConditionResult(const_cast<USystemicCondition*>(this), false, TEXT("Subject does not implement ISystemicTraitProvider.")));
		return nullptr;
	}

	// Set both parts of the script interface.
	TScriptInterface<ISystemicTraitProvider> SubjectTraitProviderInterface;
	SubjectTraitProviderInterface.SetObject(pSubjectObject);
	SubjectTraitProviderInterface.SetInterface(pSubjectTraitProvider);
	return SubjectTraitProviderInterface;
}

// Base condition evaluation; so long as it's enabled and the data is valid it'll pass.
bool USystemicCondition::Evaluate(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) const
{
	FSystemicTraceEvaluatedConditionResult result(const_cast<USystemicCondition*>(this));
	result.bResult = bEnabled && (((Subject == ESystemicEventSubject::Instigator) && Event.Instigator.IsValid())
		|| (Subject == ESystemicEventSubject::Target && Event.Target.IsValid())
		|| (Subject == ESystemicEventSubject::SourceObject && Event.SourceObject.IsValid()));
	
	if(result.bResult)
	{
		result.EvaluationLog.Add(FString::Printf(TEXT("Condition (%s): enabled.\n\tResult: Success."), *Name.ToString()));
	}
	else
	{
		result.EvaluationLog.Add(FString::Printf(TEXT("Condition (%s): enabled.\n\tResult: Failure."), *Name.ToString()));
	}

	Trace.EvaluatedConditionResults.Add(result);
	return(result.bResult);
}