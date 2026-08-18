// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "World/EnvironmentManager.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/WindDirectionalSourceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EnvironmentManager)

AEnvironmentManager::AEnvironmentManager(const FObjectInitializer& Init)
: Super(Init)
{
	// Initialize the base root component.
	if(!GetRootComponent())
	{
		RootComponent = Init.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
		RootComponent->SetMobility(EComponentMobility::Type::Movable);
	}
	
	// Initialize the root component for the directional lights.
	SunMoonRootComponent = Init.CreateDefaultSubobject<USceneComponent>(this, TEXT("SunMoonRoot"));
	SunMoonRootComponent->SetupAttachment(RootComponent);

	// Initialize the Sun and Moon directional lights.
	SunLightComponent = Init.CreateDefaultSubobject<UDirectionalLightComponent>(this, TEXT("SunLight"));
	SunLightComponent->SetupAttachment(SunMoonRootComponent);
	MoonLightComponent = Init.CreateDefaultSubobject<UDirectionalLightComponent>(this, TEXT("MoonLight"));
	MoonLightComponent->SetupAttachment(SunMoonRootComponent);

	// Give Sun forward shading priority.
	SunLightComponent->SetForwardShadingPriority(1);
	MoonLightComponent->SetForwardShadingPriority(0);
	
	// Configure other Moon default properties; most importantly differentiating the atmospheric light's secondary index.
	MoonLightComponent->SetAtmosphereSunLightIndex(1);
	MoonLightComponent->SetIntensity(0.25f);
	MoonLightComponent->SetUseTemperature(true);
	MoonLightComponent->SetTemperature(9000.f);
	MoonLightComponent->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	
	// Initialize the skylight and a couple of its properties.
	SkyLightComponent = Init.CreateDefaultSubobject<USkyLightComponent>(this, TEXT("SkyLight"));
	SkyLightComponent->SetupAttachment(RootComponent);
	SkyLightComponent->bRealTimeCapture = true;
	SkyLightComponent->bLowerHemisphereIsBlack = false;

	// Initialize the sky atmosphere.
	SkyAtmosphereComponent = Init.CreateDefaultSubobject<USkyAtmosphereComponent>(this, TEXT("SkyAtmosphere"));
	SkyAtmosphereComponent->SetupAttachment(RootComponent);

	// Initialize the volumetric cloud component.
	VolumetricCloudComponent = Init.CreateDefaultSubobject<UVolumetricCloudComponent>(this, TEXT("VolumetricClouds"));
	VolumetricCloudComponent->SetupAttachment(RootComponent);

	// Initialize the exponential height fog component and default to being volumetric fog.
	ExponentialHeightFogComponent = Init.CreateDefaultSubobject<UExponentialHeightFogComponent>(this, TEXT("ExponentialHeightFog"));
	ExponentialHeightFogComponent->SetupAttachment(RootComponent);
	ExponentialHeightFogComponent->bEnableVolumetricFog = true;

	SkySphereComponent = Init.CreateOptionalDefaultSubobject<UStaticMeshComponent>(this, TEXT("SkySphere"));
	if(SkySphereComponent)
	{
		SkySphereComponent->SetupAttachment(RootComponent);
	}

	WindDirectionalSourceComponent = Init.CreateDefaultSubobject<UWindDirectionalSourceComponent>(this, TEXT("WindDirectionalSource"));
	WindDirectionalSourceComponent->SetupAttachment(RootComponent);
}

void AEnvironmentManager::SetTimeOfDay(float TimeOfDayIn)
{
	// Set the environment's updated Time of Day.
	TimeOfDay = TimeOfDayIn;
}

#if WITH_EDITOR
void AEnvironmentManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that was modified
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Check against a specific property using the checked macro to catch typos
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnvironmentManager, TimeOfDayPreview))
	{
		// Update the preview time of day.
		TimeOfDayPreview = TimeOfDay;
	}
}
#endif