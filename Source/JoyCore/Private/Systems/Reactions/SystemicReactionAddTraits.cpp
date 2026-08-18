// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionAddTraits.h"

#include "Systems/SystemicCore.h"

#include "GameFramework/Actor.h"

#include "Systems/Rules/SystemicRuleContext.h"
#include "Systems/Traits/ISystemicTraitProvider.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SystemicReactionAddTraits)

// Add traits to the event subject.
bool USystemicReactionAddTraits::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	bool bSuccess = false;

	// Get the trait provider interface from either the event's target or the target object from the context.
	UObject* pTargetObject = Event.Target.IsValid() ? Event.Target.Get() : Context.Target.Get();
	ISystemicTraitProvider* pTraitProvider = Cast<ISystemicTraitProvider>(pTargetObject);
	if(!pTraitProvider)
	{
		// Target check failed, see if the interface is implemented by a component.
		if(const AActor* pTargetActor = Cast<AActor>(pTargetObject))
		{
			pTargetObject = pTargetActor->FindComponentByInterface(USystemicTraitProvider::StaticClass());
			pTraitProvider = Cast<ISystemicTraitProvider>(pTargetObject);
		}
	}

	if(pTraitProvider)
	{
		// Add the traits to the target.
		pTraitProvider->AddTraits(TraitTags);
		bSuccess = true;
	}
	else
	{
		// Log failure to get the trait interface.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("Reaction %s had no valid target to apply Traits to (or the Target didn't implement ISystemicTraitProvider)."), *GetReactionName().ToString());
	}

	Trace.RuleReactionNameAndResultList.Add(TPair<FName, bool>(GetReactionName(), bSuccess));
	return bSuccess;
}
