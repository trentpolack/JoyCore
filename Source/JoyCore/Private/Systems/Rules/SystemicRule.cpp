// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Rules/SystemicRule.h"

// Rule name accessor.
const FName& USystemicRule::GetRuleName() const
{
	return Name;
}

// Return a reference to the container of triggering event tags.
const FGameplayTagContainer& USystemicRule::GetTriggerEventTags() const
{
	return TriggerEventTags;
}

// Get a reference to the list of conditions.
const TArray<USystemicCondition*>& USystemicRule::GetConditionList() const
{
	return ConditionList;
}

// Get the rule priority.
const FGameplayTag& USystemicRule::GetPriority() const
{
	return Priority;
}

// Get the rule cooldown.
const float USystemicRule::GetCooldown() const
{
	return Cooldown;
}

// Get the state of this rule.
bool USystemicRule::IsEnabled() const
{
	return bEnabled;
}

// Set the state of this rule.
void USystemicRule::Enable(bool bEnabledIn)
{
	bEnabled = bEnabledIn;
}