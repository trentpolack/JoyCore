// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "PCG/JoyPCGBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(JoyPCGBlueprintLibrary)

// Create deterministic PCG points laid out in a grid; randomness is seeded per cell, so changing grid dimensions does not move existing cells.
TArray<FPCGPoint> UJoyPCGBlueprintLibrary::CreateGridPoints(const FJoyPCGGridSettings& Settings, const FTransform& Origin)
{
	// Calculate scale variation extents.
	const float scaleMin = FMath::Max(0.0, FMath::Min(Settings.ScaleRange.X, Settings.ScaleRange.Y));
	const float scaleMax = FMath::Max(scaleMin, FMath::Max(Settings.ScaleRange.X, Settings.ScaleRange.Y));
	
	// Set the grid center offset.
	const FVector offset = Settings.bCenterOnOrigin ? FVector((Settings.GridSize.X - 1)*Settings.GridPointSpacing.X*-0.5f, (Settings.GridSize.Y - 1)*Settings.GridPointSpacing.Y*-0.5f, 0.0) : FVector::ZeroVector;

	// Initialize the point array based on number of points per axis.
	TArray<FPCGPoint> points;
	points.Reserve(Settings.GridSize.X*Settings.GridSize.Y);

	// Create the grid points.
	for(int32 y = 0; y < Settings.GridSize.Y; ++y)
	{
		for(int32 x = 0; x < Settings.GridSize.X; ++x)
		{
			// Calculate the cell hash based on coordinate and either numerical or named seed.
			const uint32 cellHash = HashCombineFast(Settings.bNumericSeed ? GetTypeHash(Settings.Seed) : GetTypeHash(Settings.SeedName), HashCombineFast(GetTypeHash(x), GetTypeHash(y)));
			FRandomStream seededStream(static_cast<int32>(cellHash));

			const FVector localPosition(
				(offset.X + (x*Settings.GridPointSpacing.X)) + seededStream.FRandRange(-Settings.Jitter.X, Settings.Jitter.X),
				(offset.Y + (y*Settings.GridPointSpacing.Y)) + seededStream.FRandRange(-Settings.Jitter.Y, Settings.Jitter.Y),
				Settings.HeightOffset);

			// Apply random rotation and scale.
			const float yaw = seededStream.FRandRange(0.0, FMath::Clamp(Settings.MaximumRandomYaw, 0.0, 360.0));
			const float scale = seededStream.FRandRange(scaleMin, scaleMax);

			FPCGPoint& point = points.Emplace_GetRef();
			point.Transform = FTransform(FRotator(0.0, yaw, 0.0), localPosition, FVector(scale))*Origin;
			point.Density = FMath::Clamp(Settings.Density, 0.0f, 1.0f);
			point.Seed = static_cast<int32>(cellHash);
			point.BoundsMin = FVector(Settings.GridPointSpacing.X*-0.5f, Settings.GridPointSpacing.Y*-0.5f, -FMath::Max(0.0, Settings.BoundsHalfHeight));
			point.BoundsMax = FVector(Settings.GridPointSpacing.X*0.5f, Settings.GridPointSpacing.Y*0.5f, FMath::Max(0.0, Settings.BoundsHalfHeight));
		}
	}

	return points;
}
