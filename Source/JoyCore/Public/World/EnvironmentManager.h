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
UCLASS(Blueprintable, Category = "Game|World", HideCategories=(Collision, Cooking, HLOD, Input, Networking, Physics, Rendering), ClassGroup=(JoyCore))
class JOYCORE_API AEnvironmentManager : public AActor
{
	GENERATED_BODY()

private:
	// Time of day set from the game simulation; matches ::TimeOfDayPreview in-editor.
	UPROPERTY(Transient, AdvancedDisplay, meta=(ForceUnits="Hours"))
	float TimeOfDay = 10.0f;
	
protected:
#if WITH_EDITORONLY_DATA
	// Editor-only property to preview the environment at the specified Time of Day ([0.0, 24.0]).
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Environment|Editor", meta=(ForceUnits="Hours", DisplayName="Time of Day - Preview",ClampMin = 0.0f, UIMin=0.0f, UIMax=24.0f))
	float TimeOfDayPreview_Editor = 10.0f;

	// Editor-only property to preview the environment at the specified Time of Day ([0.0, 24.0]).
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Environment|Editor", meta=(ForceUnits="Hours", ClampMin = 0.0f, UIMin=0.0f, UIMax=24.0f))
	uint8 bTickInEditor : 1 = false;

	// Editor-only property for specifying how fast the environment should change if ticking is enabled.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Environment|Editor", meta=(EditCondition="bTickInEditor", EditConditionHides, DisplayName="Time of Day - Minutes/Second", ForceUnits="Minutes", ClampMin = 0.0f, UIMin=0.0f, UIMax=1000.0f))
	float TimeOfDayMinutesPerSecond_Editor = 10.0f;
#endif

	/**
	 *	Update the environment based on the specified time of day.
	*/
	UFUNCTION(Category = "Environment")
	virtual void UpdateEnvironment();

	/**
	 *	Update the sun and moon to represent the specified time of day.
	*/
	UFUNCTION(Category = "Environment|TimeOfDay")
	virtual void UpdateSunAndMoon();

public:
	// Root scene component shared by the sun and moon directional lights.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<USceneComponent> SunMoonRootComponent = nullptr;
	
	// Directional light source representing the Sun.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UDirectionalLightComponent> SunLightComponent = nullptr;
	// Directional light source representing the Moon.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UDirectionalLightComponent> MoonLightComponent = nullptr;

	// Skylight component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<USkyLightComponent> SkyLightComponent = nullptr;

	// Sky Atmosphere component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComponent = nullptr;

	// Volumetric Cloud component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UVolumetricCloudComponent> VolumetricCloudComponent = nullptr;

	// Exponential Height Fog component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFogComponent = nullptr;

	// Sky Sphere mesh component (optional).
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UStaticMeshComponent> SkySphereComponent = nullptr;

	// Wind Directional Source component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|Components")
	TObjectPtr<UWindDirectionalSourceComponent> WindDirectionalSourceComponent = nullptr;

	// Direction the sun/moon arc faces. Change this to rotate sunrise/sunset direction in the world.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Environment|TimeOfDay", meta = (ForceUnits = "Degrees", ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float SunRiseHour = 6.0f;

	// Direction the sun/moon arc faces (in degrees); can be changed to modify the sunrise/sunset direction in the world.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Environment|TimeOfDay", meta = (ForceUnits = "Degrees", ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float SunMoonYaw = 0.0f;

	// Pitch offset for art direction or seasonal adjustment (in degrees).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Environment|TimeOfDay", meta = (ForceUnits = "Degrees", ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float SunMoonPitchOffset = 0.0f;
	
	// Sky Sphere Radius.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Environment|SkySphere", meta = (ClampMin = "100.0", ClampMax = "25000.0", UIMin = "100.0"))
	float SkySphereRadius = 500.0f;
	
	/** Constructor. */
	AEnvironmentManager(const FObjectInitializer& Init);
	
	/** Method to set the Time of Day to update the environment visuals. */
	UFUNCTION(BlueprintCallable, Category = "Environment")
	virtual void SetTimeOfDay(UPARAM(meta = (DisplayName = "Time of Day")) float TimeOfDayIn);
	
#if WITH_EDITOR
	// AActor.
	/** Allow for ticking when in the editor. */
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	/** Tick method when in the editor. */
	virtual void Tick(float DeltaSeconds) override;
	
	/** Override the editor-only property change event. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// ~AActor.
#endif
};
