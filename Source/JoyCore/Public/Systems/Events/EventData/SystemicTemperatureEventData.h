// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicTemperatureEventData.generated.h"

/**
 *	FSystemicTemperatureEventData Structure.
 *		Event payload that's used for systemic temperature events; temperatures are in Degrees Celsius (°C).
 *		FSystemicEventData::Value represents the temperature delta.
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicTemperatureEventData : public FSystemicEventData
{
	GENERATED_BODY()
	
	// Temperature value after this event was applied.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Temperature")
	float TemperatureNew = 0.0f;

	// Temperature value before this event was applied.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Temperature")
	float TemperaturePrevious = 0.0f;
};