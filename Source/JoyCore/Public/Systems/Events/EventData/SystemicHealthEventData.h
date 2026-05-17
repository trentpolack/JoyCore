// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicHealthEventData.generated.h"

/**
 *	FSystemicHealthEventData Structure.
 *		Event payload used for systemic health events.
 *		Stores the previous and new health values associated with the event.
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicHealthEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Previous health value before the event was processed.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Health")
	float HealthPrevious = 0.0f;

	// New health value after the event was processed.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Health")
	float HealthNew = 0.0f;
};
