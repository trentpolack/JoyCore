// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "PCGPoint.h"

#include "JoyPCGTypeBase.h"

#include "JoyPCGBlueprintLibrary.generated.h"

// Content-friendly settings for creating a deterministic grid of PCG points.
USTRUCT(BlueprintType, Category = "JoyCore|PCG")
struct JOYCORE_API FJoyPCGGridSettings : public FJoyPCGTypeBase
{
	GENERATED_BODY()

	// Number of points along the X and Y axes.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid", meta = (ClampMin="1", UIMin="1"))
	FIntPoint GridSize = FIntPoint(5, 5);
	
	// Center the complete grid at the origin instead of placing its first point there.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid")
	uint8 bCenterOnOrigin : 1 = true;

	// Distance in centimeters between adjacent point centers.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid", meta = (ForceUnits="cm", ClampMin="1.0", UIMin="1.0"))
	FVector2D GridPointSpacing = FVector2D(200.0f, 200.0f);

	// Z offset applied to every generated point.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid", meta = (ForceUnits="cm"))
	float HeightOffset = 0.0f;
	
	// Half-height of each point's local bounds.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid", meta = (ForceUnits="cm", ClampMin="0.0", UIMin="0.0"))
	float BoundsHalfHeight = 100.0f;

	// Density assigned to every point.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Point", meta = (ClampMin="0.0", ClampMax="1.0", UIMin="0.0", UIMax="1.0"))
	float Density = 1.0f;

	// Maximum random offset in centimeters on each axis.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid|PCG", meta = (ForceUnits="cm", ClampMin="0.0", UIMin="0.0"))
	FVector2D Jitter = FVector2D::ZeroVector;

	// Uniform random scale range.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid|PCG", meta = (ClampMin="0.0", UIMin="0.0"))
	FVector2D ScaleRange = FVector2D(1.0f, 1.0f);

	// Randomize each point's yaw from zero up to this angle.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config|Grid|PCG", meta = (ForceUnits="deg", ClampMin="0.0", ClampMax="360.0", UIMin="0.0", UIMax="360.0"))
	float MaximumRandomYaw = 0.0f;
};

// Small, reusable PCG building blocks intended for Blueprint-authored PCG elements.
UCLASS(ClassGroup=(JoyCore), Category = "JoyCore|PCG")
class JOYCORE_API UJoyPCGBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Create deterministic PCG points laid out in a grid; randomness is seeded per cell, so changing grid dimensions does not move existing cells.
	 * @param Settings Grid settings.
	 * @param Origin Origin transform.
	 * @return Array of generated points.
	 */
	UFUNCTION(BlueprintPure, Category = "JoyCore|PCG", meta = (DisplayName="Create PCG Grid Points"))
	static TArray<FPCGPoint> CreateGridPoints(const FJoyPCGGridSettings& Settings, const FTransform& Origin);
};
