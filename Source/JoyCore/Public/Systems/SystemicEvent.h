// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

#include "SystemicEvent.generated.h"

/**
 *	Enum representing the subject of a systemic event.
 */
UENUM(BlueprintType)
enum class ESystemicEventSubject : uint8
{
	SourceObject,
	Instigator,
	Target
};

/**
 *	FSystemicEvent Structure Definition.
 */
USTRUCT(BlueprintType)
struct JOYCORE_API FSystemicEvent
{
	GENERATED_BODY()
	
	/**
	 *	FSystemicEvent. 
	 */
	
	/**
	 *	Gameplay tag associated with the event.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Markup")
	FGameplayTag EventTag = FGameplayTag();

	/**
	 *	Gameplay tags providing additional event context.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Markup")
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	/**
	 *	Initiator of the event.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<AActor> Instigator = nullptr;
	/**
	 *	Target of the event, if there is one.
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<AActor> Target = nullptr;
	/**
	 *	Source object that caused the event (for general-purpose coverage beyond actors). 
	 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime")
	TWeakObjectPtr<UObject> SourceObject = nullptr;

	/**
	 *	Location of the event, if applicable.
	 */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "Runtime")
	FVector Location = FVector::ZeroVector;
	
public:
	/**
	 *	FSystemicEvent Constructor. 
	 */
	FSystemicEvent()
	{	}
	
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
};
