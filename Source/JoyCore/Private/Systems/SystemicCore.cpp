// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicCore.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/Events/SystemicEvent.h"
#include "Systems/Rules/SystemicRule.h"

// Return the highest-priority event, defaulting to EventA if equal.
const FSystemicEvent& USystemicCore::GetHigherPriorityEvent(const FSystemicEvent& EventA, const FSystemicEvent& EventB)
{
	switch(EventA.Priority)
	{
	case TAG_System_Event_Priority_Low:
		return((EventB.Priority == TAG_System_Event_Priority_Low) ? EventA : EventB);

	case TAG_System_Event_Priority_High:
		return EventA;

	case TAG_System_Event_Priority_Default:
	case TAG_System_Event_Priority_Normal:
	default:
		// Just roll into the default return case.
		break;
	}
	
	return((EventB.Priority != TAG_System_Event_Priority_High) ? EventA : EventB);
}

// Return the highest-priority rule, defaulting to RuleA if equal.
const USystemicRule* USystemicCore::GetHigherPriorityRule(const USystemicRule* RuleA, const USystemicRule* RuleB)
{
	const FGameplayTag& ruleBPriority = RuleB->GetPriority();
	
	switch(RuleA->GetPriority())
	{
	case TAG_System_Rule_Priority_Low:
		return((ruleBPriority == TAG_System_Rule_Priority_Low) ? RuleA : RuleB);

	case TAG_System_Rule_Priority_High:
		return RuleA;

	case TAG_System_Rule_Priority_Default:
	case TAG_System_Rule_Priority_Normal:
	default:
		// Just roll into the default return case.
		break;
	}
	
	return((ruleBPriority != TAG_System_Rule_Priority_High) ? RuleA : RuleB);
}