// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SystemicWorldSubsystem.generated.h"

// Declarations.
struct FSystemicEvent;

/**
 * USystemicWorldSubsystem Class Definition.
 *	This is the subsystem that receives events from the game sim and evaluates them with Rules and Conditions to trigger an eventual response (reaction).
 */
UCLASS(Blueprintable, ClassGroup=(JoyCore))
class JOYCORE_API USystemicWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
};
