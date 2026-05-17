// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicTemperatureEventData.generated.h"

/**
 *	FSystemicTemperatureEventData Structure.
 *		Event payload used for systemic temperature events.
 *		The intended temperature unit is Degrees Celsius (°C).
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicTemperatureEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Temperature value after this event was applied.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Temperature")
	float TemperatureNew = 0.0f;

	// Temperature value before this event was applied.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Temperature")
	float TemperaturePrevious = 0.0f;
};