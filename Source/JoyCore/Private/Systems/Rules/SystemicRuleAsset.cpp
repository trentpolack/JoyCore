// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Rules/SystemicRuleAsset.h"

// Rule name accessor.
const FName& USystemicRuleAsset::GetRuleName() const
{
	return Name;
}

// Return a reference to the container of triggering event tags.
const FGameplayTagContainer& USystemicRuleAsset::GetTriggerEventTags() const
{
	return TriggerEventTags;
}

// Get a reference to the list of conditions.
const TArray<USystemicCondition*>& USystemicRuleAsset::GetConditionList() const
{
	return ConditionList;
}

// Get the rule priority.
const FGameplayTag& USystemicRuleAsset::GetPriority() const
{
	return Priority;
}

// Get the rule cooldown.
const float USystemicRuleAsset::GetCooldown() const
{
	return Cooldown;
}

// Get the state of this rule.
bool USystemicRuleAsset::IsEnabled() const
{
	return bEnabled;
}

// Set the state of this rule.
void USystemicRuleAsset::Enable(bool bEnabledIn)
{
	bEnabled = bEnabledIn;
}