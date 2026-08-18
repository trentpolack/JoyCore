// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Rules/SystemicRule.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SystemicRule)

// Rule name accessor.
const FName& USystemicRule::GetRuleName() const
{
	return RuleName;
}

// Rule Description accessor.
const FString& USystemicRule::GetRuleDescription() const
{
	return RuleDescription;
}

// Return a reference to the container of triggering event tags.
const FGameplayTagContainer& USystemicRule::GetTriggerEventTags() const
{
	return TriggerEventTags;
}

// Get a reference to the list of conditions.
const TArray<TObjectPtr<USystemicCondition>>& USystemicRule::GetConditionList() const
{
	return ConditionList;
}

// Get a reference to the list of reactions.
const TArray<TObjectPtr<USystemicReaction>>& USystemicRule::GetReactionList() const
{
	return ReactionList;
}

// Get the rule priority.
const FGameplayTag& USystemicRule::GetPriority() const
{
	return Priority;
}

// Get the rule cooldown.
float USystemicRule::GetCooldown() const
{
	return Cooldown;
}

// Get the state of this rule.
bool USystemicRule::GetIsEnabled() const
{
	return bEnabled;
}

// Set the state of this rule.
bool USystemicRule::Enable(bool bEnabledIn)
{
	bEnabled = bEnabledIn;
	return bEnabledIn;
}