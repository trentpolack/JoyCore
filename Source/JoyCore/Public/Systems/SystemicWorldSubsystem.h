// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SystemicWorldSubsystem.generated.h"

// Declarations.
class USystemicRuleAsset;

struct FSystemicEvent;

/**
 * 
 */
UCLASS(ClassGroup=(JoyCore), meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
};
