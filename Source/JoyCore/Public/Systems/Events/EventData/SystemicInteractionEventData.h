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
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicInteractionEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Name of the interaction associated with this event.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Event|Data|Interaction|Transient")
	FName InteractionName = NAME_None;
};