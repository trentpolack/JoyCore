// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicHealthEventData.generated.h"

/**
 *	FSystemicHealthEventData Structure.
 *		Event payload used for systemic health events.
 *		FSystemicEventData::Value represents the health delta.
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicHealthEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Previous health value before the event was processed.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Health")
	float HealthPrevious = 0.0f;

	// New health value after the event was processed.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Health")
	float HealthNew = 0.0f;
};
