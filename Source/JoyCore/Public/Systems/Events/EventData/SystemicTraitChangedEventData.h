// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicTraitChangedEventData.generated.h"

/**
 *	FSystemicTraitChangedEventData Structure.
 *		Event payload that's used for general changes in gameplay tags (particularly traits and states).
 *		FSystemicEventData::Value is unused.
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicTraitChangedEventData : public FSystemicEventData
{
	GENERATED_BODY()
	
	// Gameplay tag container after this event was applied.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient, AdvancedDisplay, Category="EventData")
	FGameplayTagContainer TraitsNew = FGameplayTagContainer();

	// Gameplay tag container before this event was applied.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient, AdvancedDisplay, Category="EventData")
	FGameplayTagContainer TraitsPrevious = FGameplayTagContainer();
};