// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Events/SystemicEvent.h"

#include "SystemicCore.generated.h"

// Declarations.
class USystemicRule;

struct FSystemicTrace;

struct FSystemicEvent;
struct FSystemicContactEventData;
struct FSystemicHealthEventData;
struct FSystemicInteractionEventData;
struct FSystemicTemperatureEventData;

// Log declaration.
DECLARE_LOG_CATEGORY_EXTERN(LogJoyCoreSystems, Log, All);

/**
 * USystemicCore Class Definition.
 *	Core config settings, helpers, and utilities. Not intended to be instanced.
 */
UCLASS(NotBlueprintable, NotBlueprintType, Category="Game|Systems", ClassGroup=(JoyCore))
class JOYCORE_API USystemicCore : public UObject
{
	GENERATED_BODY()

public:
	/**
	 *	Traits.
	 */

	/**
	 *	Get the description of the trait query.
	 *	@returns Description of the trait query.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Traits", meta=(GameplayTagFilter=TAG_System_Trait))
	static const FString& GetTraitQueryDescription(const FGameplayTagQuery& TraitTagQuery)
	{
		return(TraitTagQuery.GetDescription());
	}

	/**
	 *	Events.
	 */
	
	/**
	 * Get the object associated with the event condition's subject.
	 * @param Event The event to retrieve the subject object from.
	 * @param Subject The subject type to retrieve the object for.
	 * @returns The object if found, otherwise nullptr.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Events")
	static UObject* GetEventObjectBySubject(const FSystemicEvent& Event, const ESystemicEventSubject Subject)
	{
		switch(Subject)
		{
		case ESystemicEventSubject::Instigator:		return(Cast<UObject>(Event.Instigator.Get()));
		case ESystemicEventSubject::Target:			return(Cast<UObject>(Event.Target.Get()));
		case ESystemicEventSubject::Source:			return(Cast<UObject>(Event.Source.Get()));
		}

		return nullptr;
	}

	/**
	 * Get which of the two passed-in Events is higher-priority.
	 * @param EventA First Event to compare
	 * @param EventB Second Event to compare
	 * @returns The Event with higher priority; EventA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Events")
	static const FSystemicEvent& GetHigherPriorityEvent(const FSystemicEvent& EventA, const FSystemicEvent& EventB);

	/**
	 * Get the event data from the passed-in Event as a FSystemicContactEventData-typed struct.
	 * @param Event Contact Event to extract data from.
	 * @param EventDataOut Output struct to populate with event data.
	 * @returns True if the event is a contact-related event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Systems|Events")
	static bool GetContactEventData(FSystemicEvent& Event, FSystemicContactEventData& EventDataOut);
	
	/**
	 * Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
	 * @param Event Health Event to extract data from.
	 * @param EventDataOut Output struct to populate with event data.
	 * @returns True if the event is a health-related event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Systems|Events")
	static bool GetHealthEventData(FSystemicEvent& Event, FSystemicHealthEventData& EventDataOut);

	/**
	 * Get the event data from the passed-in Event as a FSystemicInteractionEventData-typed struct.
	 * @param Event Interaction Event to extract data from.
	 * @param EventDataOut Output struct to populate with event data.
	 * @returns True if the event is an interaction-related event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Systems|Events")
	static bool GetInteractionEventData(FSystemicEvent& Event, FSystemicInteractionEventData& EventDataOut);

	/**
	 * Get the event data from the passed-in Event as a FSystemicTemperatureEventData-typed struct.
	 * @param Event Temperature Event to extract data from.
	 * @param EventDataOut Output struct to populate with event data.
	 * @returns True if the event is a temperature-related event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Systems|Events")
	static bool GetTemperatureEventData(FSystemicEvent& Event, FSystemicTemperatureEventData& EventDataOut);
	
	/**
	 *	Rules.
	 */

	/**
	 * Get which of the two passed-in rules is higher-priority.
	 * @param RuleA First rule to compare
	 * @param RuleB Second rule to compare
	 * @returns The rule with higher priority; RuleA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Rules")
	static const USystemicRule* GetHigherPriorityRule(const USystemicRule* RuleA, const USystemicRule* RuleB);
};