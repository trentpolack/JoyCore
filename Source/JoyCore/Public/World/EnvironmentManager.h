// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentManager.generated.h"

// Declarations.
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;
class USkyLightComponent;
class UWindDirectionalSourceComponent;
class UVolumetricCloudComponent;

/**
 * AEnvironmentManager Class Definition.
 *	Manages core environment properties and related visual components (lighting, volumetrics, etc.); actual game sim logic is handled elsewhere.
 */
UCLASS(Blueprintable, Category="Game|World", ClassGroup=(JoyCore))
class JOYCORE_API AEnvironmentManager : public AActor
{
	GENERATED_BODY()

private:
	// Time of day set from the game simulation; matches ::TimeOfDayPreview in-editor.
	UPROPERTY(Transient, AdvancedDisplay, meta=(Units="Hours"))
	float TimeOfDay = 10.0f;
	
protected:
#if WITH_EDITORONLY_DATA
	// Editor-only property to preview the environment at the specified Time of Day ([0.0, 24.0]).
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment", meta=(ForceUnits="Hours", ClampMin = 0.0f, UIMin=0.0f, UIMax=24.0f))
	float TimeOfDayPreview = 10.0f;
#endif

public:
	// Root scene component shared by the sun and moon directional lights.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<USceneComponent> SunMoonRootComponent = nullptr;
	
	// Directional light source representing the Sun.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UDirectionalLightComponent> SunLightComponent = nullptr;
	// Directional light source representing the Moon.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UDirectionalLightComponent> MoonLightComponent = nullptr;

	// Skylight component.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<USkyLightComponent> SkyLightComponent = nullptr;

	// Sky Atmosphere component.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComponent = nullptr;

	// Volumetric Cloud component.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UVolumetricCloudComponent> VolumetricCloudComponent = nullptr;

	// Exponential Height Fog component.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFogComponent = nullptr;

	// Sky Sphere mesh component (optional).
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UStaticMeshComponent> SkySphereComponent = nullptr;

	// Wind Directional Source component.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="Environment|Components")
	TObjectPtr<UWindDirectionalSourceComponent> WindDirectionalSourceComponent = nullptr;
	
	/** Constructor. */
	AEnvironmentManager(const FObjectInitializer& Init);
	
	/** Method to set the Time of Day to update the environment visuals. */
	UFUNCTION(BlueprintCallable, Category="Environment")
	virtual void SetTimeOfDay(UPARAM(meta=(DisplayName="Time of Day"))float TimeOfDayIn);
	
#if WITH_EDITOR
	// Override the editor-only property change event
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
