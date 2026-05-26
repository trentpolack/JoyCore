// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Engine/HitResult.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicContactEventData.generated.h"

/**
*	FSystemicContactEventData Structure.
 *		Event payload used for systemic contact events.
 *		Stores contact surface, impulse, and bone/socket context for collision-like interactions.
 *		FSystemicEventData::Value represents the magnitude of the collision event (irrelevant for overlaps).
 */
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicContactEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Hit result from the contact event.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="EventData|Contact")
	FHitResult HitResult = FHitResult();

	// Basic constructor.
	FSystemicContactEventData()
	{	}

	// Constructor required to assign HitResult.
	FSystemicContactEventData(const FHitResult& HitResultIn)
	: HitResult(HitResultIn)
	{	}
};