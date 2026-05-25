// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicInteractionEventData.generated.h"

/**
 *	FSystemicInteractionEventData Structure.
 *		Event payload used for systemic interaction events.
 *		Stores the interaction name associated with the emitted systemic event.
 */
USTRUCT()
struct JOYCORE_API FSystemicInteractionEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Name of the interaction associated with this event.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, AdvancedDisplay, Category="EventData|Interaction")
	FName InteractionName = NAME_None;
};