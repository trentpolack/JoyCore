// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"
#include "Templates/UniquePtr.h"

#include "GameplayTags.h"

#include "JoyCoreNativeGameplayTags.h"

#include "SystemicEvent.generated.h"

#define JOYCORE_POPULATE_EVENT(Event, EventTagIn, OwnerIn, InstigatorActorIn, SourceIn, EventDataTypeIn)		\
	Event.EventTag = EventTagIn;									\
	Event.Target = OwnerIn;											\
	Event.Instigator = InstigatorActorIn;							\
	Event.Source = SourceIn ? SourceIn : this;						\
	Event.EventDataInstance.InitializeAs<EventDataTypeIn>()

#define JOYCORE_POPULATE_EVENT_CONSTRUCTOR_ARGLIST(Event, EventTagIn, OwnerIn, InstigatorActorIn, SourceIn, EventDataTypeIn, ...)		\
	Event.EventTag = EventTagIn;									\
	Event.Target = OwnerIn;											\
	Event.Instigator = InstigatorActorIn;							\
	Event.Source = SourceIn ? SourceIn : this;						\
	Event.EventDataInstance.InitializeAs<EventDataTypeIn>(__VA_ARGS__)

/**
 *	Enum representing the subject of a systemic event.
 */
UENUM(BlueprintType, Category="Game|Systems")
enum class ESystemicEventSubject : uint8
{
	Source,
	Instigator,
	Target
};

/**
 *	FSystemicEventData Structure.
 *		Event payload that's used as the base for systemic event data
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct FSystemicEventData
{
	GENERATED_BODY()
	
	// Location of the event, if applicable.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient, Category="EventData")
	FVector Location = FVector::ZeroVector;
	
	// General-purpose delta value associated with the event, if applicable; e.g., health lost, temperature change, etc.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category="EventData")
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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(GameplayTagFilter="System.Event", DisplayPriority="1"))
	FGameplayTag EventTag = FGameplayTag();
	
	// Priority tag for this event (unused right now, 4/5/26).
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(GameplayTagFilter="System.Event.Priority", DisplayAfter="EventTag"))
	FGameplayTag Priority = TAG_System_Event_Priority_Default;

	// Gameplay tags providing additional event context (unused right now, 5/25/26).
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category="Event", meta=(GameplayTagFilter="System", DisplayAfter="Priority"))
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	// Subject of this event.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(DisplayAfter="ContextTags"))
	ESystemicEventSubject Subject = ESystemicEventSubject::Target;
	
	// Initiator of the event.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(DisplayAfter="Subject"))
	TWeakObjectPtr<AActor> Instigator = nullptr;
	// Target of the event, if there is one.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(DisplayAfter="Subject"))
	TWeakObjectPtr<UObject> Target = nullptr;
	// Source that caused the event (for general-purpose coverage beyond actors). 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Event", meta=(DisplayAfter="Subject"))
	TWeakObjectPtr<UObject> Source = nullptr;

	// Instance of the EventDataStruct.
	UPROPERTY(BlueprintReadWrite, Transient, Category="Event", meta=(BaseStruct="/Script/JoyCore.SystemicEventData"))
	FInstancedStruct EventDataInstance = FInstancedStruct(FSystemicEventData::StaticStruct());

public:
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicEvent()
	{
	}

	/**
	 *	Check if the event data is valid for the specified type.
	 *	@returns True if the event data is valid for the specified type, false otherwise.
	 */
	template <typename T>
	FORCEINLINE bool IsValidEventDataType() const
	{
		return(EventDataInstance.GetPtr<T>() != nullptr);
	}

	/**
	 *	Templated accessor for the FSystemicEventData instance.
	 *	@returns Reference to the event data instance.
	 */
	template <typename T>
	FORCEINLINE T& GetEventDataMutable()
	{
		ensure(EventDataInstance.IsValid());
		return EventDataInstance.GetMutable<T>();
	}

	template <typename T>
	FORCEINLINE const T& GetEventData() const
	{
		ensure(EventDataInstance.IsValid());
		return EventDataInstance.Get<T>();
	}
};
