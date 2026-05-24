// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicCore.h"

#include "Systems/SystemicGameplayTags.h"

#include "Systems/Events/SystemicEvent.h"
#include "Systems/Events/EventData/SystemicContactEventData.h"
#include "Systems/Events/EventData/SystemicHealthEventData.h"
#include "Systems/Events/EventData/SystemicInteractionEventData.h"
#include "Systems/Events/EventData/SystemicTemperatureEventData.h"

#include "Systems/Rules/SystemicRule.h"

// Define the log category for the JoyCore systems logic.
DEFINE_LOG_CATEGORY(LogJoyCoreSystems);

// Return the highest-priority event, defaulting to EventA if equal.
const FSystemicEvent& USystemicCore::GetHigherPriorityEvent(const FSystemicEvent& EventA, const FSystemicEvent& EventB)
{
	if(EventA.Priority == TAG_System_Event_Priority_High)
		return EventA;
	else if(EventA.Priority == TAG_System_Event_Priority_Low)
		return((EventB.Priority == TAG_System_Event_Priority_Low) ? EventA : EventB);

	// Condition handling for default and normal.
	return((EventB.Priority != TAG_System_Event_Priority_High) ? EventA : EventB);
}

// Get the event data from the passed-in Event as a FSystemicContactEventData-typed struct.
bool USystemicCore::GetContactEventData(FSystemicEvent& Event, FSystemicContactEventData& EventDataOut)
{
	if(!Event.IsValidEventDataType<FSystemicContactEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetContactEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	EventDataOut = Event.GetEventDataMutable<FSystemicContactEventData>();	
	return true;
}

// Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
bool USystemicCore::GetHealthEventData(FSystemicEvent& Event, FSystemicHealthEventData& EventDataOut)
{
	if(!Event.IsValidEventDataType<FSystemicHealthEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetHealthEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	EventDataOut = Event.GetEventDataMutable<FSystemicHealthEventData>();	
	return true;
}

// Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
bool USystemicCore::GetInteractionEventData(FSystemicEvent& Event, FSystemicInteractionEventData& EventDataOut)
{
	if(!Event.IsValidEventDataType<FSystemicInteractionEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetInteractionEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	EventDataOut = Event.GetEventDataMutable<FSystemicInteractionEventData>();	
	return true;
}

// Get the event data from the passed-in Event as a FSystemicTemperatureEventData-typed struct.
bool USystemicCore::GetTemperatureEventData(FSystemicEvent& Event, FSystemicTemperatureEventData& EventDataOut)
{
	if(!Event.IsValidEventDataType<FSystemicTemperatureEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetTemperatureEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	EventDataOut = Event.GetEventDataMutable<FSystemicTemperatureEventData>();	
	return true;
}

// Return the highest-priority rule, defaulting to RuleA if equal.
const USystemicRule* USystemicCore::GetHigherPriorityRule(const USystemicRule* RuleA, const USystemicRule* RuleB)
{
	const FGameplayTag& ruleAPriority = RuleA->GetPriority();
	const FGameplayTag& ruleBPriority = RuleB->GetPriority();
	
	if(ruleAPriority == TAG_System_Rule_Priority_High)
		return RuleA;
	else if(ruleAPriority == TAG_System_Rule_Priority_Low)
		return((ruleBPriority == TAG_System_Rule_Priority_Low) ? RuleA : RuleB);
	
	return((ruleBPriority != TAG_System_Rule_Priority_High) ? RuleA : RuleB);
}