// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"

#include "GameplayTags.h"

#include "Systems/SystemicGameplayTags.h"

#include "SystemicEvent.generated.h"

#define JOYCORE_POPULATE_EVENT(Event, EventTagIn, OwnerIn, InstigatorActorIn, SourceObjectIn, EventDataTypeIn)		\
	Event.EventTag = EventTagIn;									\
	Event.Target = OwnerIn;											\
	Event.Instigator = InstigatorActorIn;							\
	Event.SourceObject = SourceObjectIn ? SourceObjectIn : this;	\
	Event.EventDataInstance.InitializeAs<EventDataTypeIn>()

#define JOYCORE_POPULATE_EVENT_CONSTRUCTOR_ARGLIST(Event, EventTagIn, OwnerIn, InstigatorActorIn, SourceObjectIn, EventDataTypeIn, ...)		\
	Event.EventTag = EventTagIn;									\
	Event.Target = OwnerIn;											\
	Event.Instigator = InstigatorActorIn;							\
	Event.SourceObject = SourceObjectIn ? SourceObjectIn : this;	\
	Event.EventDataInstance.InitializeAs<EventDataTypeIn>(__VA_ARGS__)

/**
 *	Enum representing the subject of a systemic event.
 */
UENUM(BlueprintType, Category="Game|Systems")
enum class ESystemicEventSubject : uint8
{
	SourceObject,
	Instigator,
	Target
};

USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct FSystemicEventData
{
	GENERATED_BODY()
	
	// Location of the event, if applicable.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "EventData")
	FVector Location = FVector::ZeroVector;
	
	// General-purpose delta value associated with the event, if applicable; e.g. health lost, temperature change, etc.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "EventData")
	float Value = 1.0f;
};

/**
 *	FSystemicEvent Structure Definition.
 */
USTRUCT(BlueprintType, Category="Game|Systems")
struct JOYCORE_API FSystemicEvent
{
	GENERATED_BODY()
	
	// Gameplay tag associated with the event.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient", meta=(GameplayTagFilter="System.Event"))
	FGameplayTag EventTag = FGameplayTag();
	
	// Priority tag for this event (unused right now, 4/5/26).
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient", meta=(GameplayTagFilter="System.Event.Priority"))
	FGameplayTag Priority = TAG_System_Event_Priority_Default;

	// Gameplay tags providing additional event context.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient")
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	// Initiator of the event.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient")
	TWeakObjectPtr<AActor> Instigator = nullptr;
	// Target of the event, if there is one.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient")
	TWeakObjectPtr<AActor> Target = nullptr;
	// Source object that caused the event (for general-purpose coverage beyond actors). 
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient")
	TWeakObjectPtr<UObject> SourceObject = nullptr;
	// Instance of the EventDataStruct.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category = "Event|Transient")
	TInstancedStruct<FSystemicEventData> EventDataInstance;

public:
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicEvent()
	{
	}
};
