// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Events/SystemicEvent.h"

#include "SystemicCore.generated.h"

// Declarations.
class USystemicRule;

struct FSystemicEvent;
struct FSystemicTrace;

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
	 *	Events.
	 */
	
	/**
	 * Get the object associated with the event condition's subject.
	 * @param Event The event to retrieve the subject object from.
	 * @param Subject The subject type to retrieve the object for.
	 * @return The object if found, otherwise nullptr.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Events")
	static UObject* GetEventObjectBySubject(const FSystemicEvent& Event, const ESystemicEventSubject Subject)
	{
		switch(Subject)
		{
		case ESystemicEventSubject::Instigator:		return(Cast<UObject>(Event.Instigator.Get()));
		case ESystemicEventSubject::Target:			return(Cast<UObject>(Event.Target.Get()));
		case ESystemicEventSubject::SourceObject:	return(Cast<UObject>(Event.SourceObject.Get()));
		}

		return nullptr;
	}

	/**
	 * Get which of the two passed-in Events is higher-priority.
	 * @param EventA First Event to compare
	 * @param EventB Second Event to compare
	 * @return The Event with higher priority; EventA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Events")
	static const FSystemicEvent& GetHigherPriorityEvent(const FSystemicEvent& EventA, const FSystemicEvent& EventB);
	
	/**
	 *	Rules.
	 */

	/**
	 * Get which of the two passed-in rules is higher-priority.
	 * @param RuleA First rule to compare
	 * @param RuleB Second rule to compare
	 * @return The rule with higher priority; RuleA if priority is equal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Game|Systems|Rules")
	static const USystemicRule* GetHigherPriorityRule(const USystemicRule* RuleA, const USystemicRule* RuleB);
};