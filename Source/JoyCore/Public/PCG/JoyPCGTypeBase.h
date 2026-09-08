// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "JoyPCGTypeBase.generated.h"

// Base type data for any PCG structure.
USTRUCT(BlueprintType, Category = "JoyCore|PCG")
struct JOYCORE_API FJoyPCGTypeBase
{
	GENERATED_BODY()

	// Seed used for per-cell variation; existing cells remain stable when the grid grows.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|PCG")
	uint8 bNumericSeed : 1 = false;

	// String-based seed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|PCG", meta = (EditCondition="!bNumericSeed", EditConditionHides))
	FString SeedName = FString(TEXT("JoyPCGSeed"));

	// Numeric seed (instead of a string-based one).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|PCG", meta = (EditCondition="bNumericSeed", EditConditionHides))
	int32 Seed = 0451;
};