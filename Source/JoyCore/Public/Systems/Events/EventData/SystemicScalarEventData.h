// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "Systems/Events/SystemicEvent.h"

#include "SystemicScalarEventData.generated.h"

// Tagged scalar change shared by needs, resources, and other numeric systems.
USTRUCT(BlueprintType, Category="Game|Systems|EventData")
struct JOYCORE_API FSystemicScalarEventData : public FSystemicEventData
{
	GENERATED_BODY()

	// Exact identifier of the value that changed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Transient|EventData|Scalar")
	FGameplayTag ScalarTag = FGameplayTag::EmptyTag;

	// Value before the operation.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Transient|EventData|Scalar")
	float ValuePrevious = 0.0f;

	// Clamped value after the operation.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Transient|EventData|Scalar")
	float ValueNew = 0.0f;
};
