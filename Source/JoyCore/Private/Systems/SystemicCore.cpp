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

// Update the event data of the passed-in event instance with the provided event data.
bool USystemicCore::UpdateEventData(FSystemicEvent& Event, const FInstancedStruct& EventData)
{
	if(!EventData.IsValid() || (Event.EventDataInstance.GetScriptStruct() != EventData.GetScriptStruct()))
	{
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::UpdateEventData failed due to invalid event data (event: %s)."), *Event.EventTag.ToString());
		return false;
	}
	
	Event.EventDataInstance = EventData;
	return true;
}

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
const FSystemicContactEventData& USystemicCore::GetContactEventData(const FSystemicEvent& Event)
{
	if(!Event.IsValidEventDataType<FSystemicContactEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetContactEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
	}
	
	return(Event.GetEventData<FSystemicContactEventData>());	
}

// Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
const FSystemicHealthEventData& USystemicCore::GetHealthEventData(const FSystemicEvent& Event)
{
	if(!Event.IsValidEventDataType<FSystemicHealthEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetHealthEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
	}
	
	return(Event.GetEventData<FSystemicHealthEventData>());	
}

// Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
const FSystemicInteractionEventData& USystemicCore::GetInteractionEventData(const FSystemicEvent& Event)
{
	if(!Event.IsValidEventDataType<FSystemicInteractionEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetInteractionEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
	}
	
	return(Event.GetEventData<FSystemicInteractionEventData>());
}

// Get the event data from the passed-in Event as a FSystemicTemperatureEventData-typed struct.
const FSystemicTemperatureEventData& USystemicCore::GetTemperatureEventData(const FSystemicEvent& Event)
{
	if(!Event.IsValidEventDataType<FSystemicTemperatureEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetTemperatureEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
	}
	
	return(Event.GetEventData<FSystemicTemperatureEventData>());
}

// Get the event data from the passed-in Event as a FSystemicTraitChangedEventData-typed struct.
const FSystemicTraitChangedEventData& USystemicCore::GetTraitChangedEventData(const FSystemicEvent& Event)
{
	if(!Event.IsValidEventDataType<FSystemicTraitChangedEventData>())
	{
		// Invalid event data type.
		UE_LOG(LogJoyCoreSystems, Error, TEXT("USystemicCore::GetTraitChangedEventData type mismatch (event: %s)."), *Event.EventTag.ToString());
	}
	
	return(Event.GetEventData<FSystemicTraitChangedEventData>());
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