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
enum class ESystemicSubject : uint8
{
	Instigator,
	Target,
	SourceObject
};

/**
 *	FSystemEvent Structure Definition.
 */
USTRUCT(BlueprintType, ClassGroup=(JoyCore))
struct JOYCORE_API FSystemicEvent
{
	GENERATED_BODY()
	
	/**
	 *	FSystemEvent. 
	 */
	
	/**
	 *	Gameplay tag associated with the event.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Markup")
	FGameplayTag EventTag;
	/**
	 *	Gameplay tags providing additional event context.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Markup")
	FGameplayTagContainer ContextTags;

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
	
	/**
	 *	FSystemEvent Constructor. 
	 */
	FSystemicEvent()
	{	}
};
