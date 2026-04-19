// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Reactions/SystemicReactionAddTraits.h"

#include "Systems/SystemicCore.h"

#include "Systems/Rules/SystemicRuleContext.h"
#include "Systems/Traits/ISystemicTraitProvider.h"

// Add traits to the event subject.
bool USystemicReactionAddTraits::Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace)
{
	bool bSuccess = false;
	if (ISystemicTraitProvider* pTraitProvider = Cast<ISystemicTraitProvider>(Context.Target))
	{
		pTraitProvider->AddTraitTags(TraitTags);
		
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