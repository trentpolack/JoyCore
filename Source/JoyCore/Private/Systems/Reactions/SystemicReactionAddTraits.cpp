// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionAddTraits.h"

#include "Systems/SystemicCore.h"

#include "GameFramework/Actor.h"

#include "Systems/Rules/SystemicRuleContext.h"
#include "Systems/Traits/ISystemicTraitProvider.h"

// Add traits to the event subject.
bool USystemicReactionAddTraits::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	bool bSuccess = false;

	// Get the trait provider interface from the target object.
	UObject* TargetObject = Context.Target.Get();
	ISystemicTraitProvider* pTraitProvider = Cast<ISystemicTraitProvider>(TargetObject);
	if(!pTraitProvider)
	{
		// Target check failed, see if the interface is implemented by a component.
		if(const AActor* pTargetActor = Cast<AActor>(TargetObject))
		{
			TargetObject = pTargetActor->FindComponentByInterface(USystemicTraitProvider::StaticClass());
			pTraitProvider = Cast<ISystemicTraitProvider>(TargetObject);
		}
	}

	if(pTraitProvider)
	{
		pTraitProvider->AddTraits(TraitTags);
		
		bSuccess = true;
	}
	else
	{
		// Log failure to get the trait interface.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Reaction %s had no valid target to apply Traits to (or the Target didn't implement ISystemicTraitProvider)."), *ReactionName.ToString());
	}

	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(ReactionName, bSuccess));
	return bSuccess;
}
