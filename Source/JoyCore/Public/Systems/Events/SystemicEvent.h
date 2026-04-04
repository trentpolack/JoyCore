// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"
#include "GameplayTags.h"

#include "SystemicEvent.generated.h"

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

USTRUCT(BlueprintType, Category="Game|Systems")
struct FSystemicEventData
{
	GENERATED_BODY()
	
	// Location of the event, if applicable.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "Event|Data")
	FVector Location = FVector::ZeroVector;
	
	// Magnitude of the event, if applicable.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "Event|Data")
	float Magnitude = 1.0f;
};

/**
 *	FSystemicEvent Structure Definition.
 */
USTRUCT(BlueprintType, Category="Game|Systems")
struct JOYCORE_API FSystemicEvent
{
	GENERATED_BODY()
	
	// Gameplay tag associated with the event.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event")
	FGameplayTag EventTag = FGameplayTag();

	// Gameplay tags providing additional event context.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event")
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	// Initiator of the event.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Runtime")
	TWeakObjectPtr<AActor> Instigator = nullptr;
	// Target of the event, if there is one.
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Runtime")
	TWeakObjectPtr<AActor> Target = nullptr;
	// Source object that caused the event (for general-purpose coverage beyond actors). 
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly, Category = "Runtime")
	TWeakObjectPtr<UObject> SourceObject = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	TInstancedStruct<FSystemicEventData> ContextData;

public:
	/**
	 * Get the object associated with the condition's subject.
	 * @return The object if found, otherwise nullptr.
	 */
	const TWeakObjectPtr<UObject> GetObjectBySubject(const ESystemicEventSubject Subject) const
	{
		switch(Subject)
		{
			case ESystemicEventSubject::Instigator:		return(Cast<UObject>(Instigator.Get()));
			case ESystemicEventSubject::Target:			return(Cast<UObject>(Target.Get()));
			case ESystemicEventSubject::SourceObject:	return(Cast<UObject>(SourceObject.Get()));
		}

		return nullptr;
	}

	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicEvent()
	{	}
};
