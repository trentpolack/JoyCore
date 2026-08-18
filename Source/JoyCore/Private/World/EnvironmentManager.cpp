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
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
	
	// Initialize the base root component.
	if(!GetRootComponent())
	{
		RootComponent = Init.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
		RootComponent->SetMobility(EComponentMobility::Type::Movable);
	}
	
	// Initialize the root component for the directional lights.
	SunMoonRootComponent = Init.CreateDefaultSubobject<USceneComponent>(this, TEXT("SunMoonRoot"));
	SunMoonRootComponent->SetMobility(EComponentMobility::Type::Movable);
	SunMoonRootComponent->SetupAttachment(RootComponent);

	// Initialize the Sun and Moon directional lights.
	SunLightComponent = Init.CreateDefaultSubobject<UDirectionalLightComponent>(this, TEXT("SunLight"));
	SunLightComponent->SetMobility(EComponentMobility::Type::Movable);
	SunLightComponent->SetupAttachment(SunMoonRootComponent);
	MoonLightComponent = Init.CreateDefaultSubobject<UDirectionalLightComponent>(this, TEXT("MoonLight"));
	MoonLightComponent->SetMobility(EComponentMobility::Type::Movable);
	MoonLightComponent->SetupAttachment(SunMoonRootComponent);

	// Set initial properties for the sun directional light.
	SunLightComponent->SetAtmosphereSunLight(true);
	SunLightComponent->SetAtmosphereSunLightIndex(0);
	SunLightComponent->SetForwardShadingPriority(1);
	SunLightComponent->bCastCloudShadows = true;
	SunLightComponent->bPerPixelAtmosphereTransmittance = true;
	SunLightComponent->CloudShadowExtent = 25.0f;
	
	// Configure other Moon default properties; most importantly differentiating the atmospheric light's secondary index.
	MoonLightComponent->SetAtmosphereSunLight(true);
	MoonLightComponent->SetAtmosphereSunLightIndex(1);
	MoonLightComponent->SetForwardShadingPriority(0);
	MoonLightComponent->SetIntensity(0.25f);
	MoonLightComponent->SetUseTemperature(true);
	MoonLightComponent->SetTemperature(9000.f);
	MoonLightComponent->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	MoonLightComponent->bCastCloudShadows = true;
	MoonLightComponent->bPerPixelAtmosphereTransmittance = true;
	MoonLightComponent->CloudShadowExtent = 25.0f;
	
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
	ExponentialHeightFogComponent->SetMobility(EComponentMobility::Type::Movable);
	ExponentialHeightFogComponent->SetupAttachment(RootComponent);
	ExponentialHeightFogComponent->bEnableVolumetricFog = true;

	// Sky Sphere is an optional default subobject and, if it's created, fill out some of its properties.
	SkySphereComponent = Init.CreateOptionalDefaultSubobject<UStaticMeshComponent>(this, TEXT("SkySphere"));
	if(SkySphereComponent)
	{
		SkySphereComponent->SetMobility(EComponentMobility::Type::Movable);
		SkySphereComponent->SetCollisionProfileName("NoCollision", false);
		SkySphereComponent->SetRelativeScale3D(FVector(SkySphereRadius));
		SkySphereComponent->SetupAttachment(RootComponent);

		SkySphereComponent->SetCastShadow(false);
	}

	// Initialize the wind directional source component.
	WindDirectionalSourceComponent = Init.CreateDefaultSubobject<UWindDirectionalSourceComponent>(this, TEXT("WindDirectionalSource"));
	WindDirectionalSourceComponent->SetMobility(EComponentMobility::Type::Movable);
	WindDirectionalSourceComponent->SetupAttachment(RootComponent);
}

void AEnvironmentManager::SetTimeOfDay(float TimeOfDayIn)
{
	// Set the environment's updated Time of Day.
	TimeOfDay = TimeOfDayIn;
}

#if WITH_EDITOR
bool AEnvironmentManager::ShouldTickIfViewportsOnly() const
{
	UWorld* pWorld = GetWorld();

	if(pWorld && bTickInEditor && (pWorld->WorldType == EWorldType::Editor))
	{
		// Only advance the time of day if ::bTickInEditor and the world is in the editor state. 
		return true;	
	}
	
	return false;
}

void AEnvironmentManager::Tick(float DeltaSeconds)
{
	// Only advance the time of day if ::bTickInEditor and the world is in the editor state. 
	if(ShouldTickIfViewportsOnly())
	{
		// This uses the same calculation that the WorldSimulationComponent does.
		SetTimeOfDay(FMath::Wrap<float>(TimeOfDay + (DeltaSeconds*TimeOfDayPreview_Editor)/60.0f, 0.0f, 24.0f));
	}
}

void AEnvironmentManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that was modified
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Check against a specific property using the checked macro to catch typos
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnvironmentManager, TimeOfDayPreview_Editor))
	{
		// Update the preview time of day.
		SetTimeOfDay(TimeOfDayPreview_Editor);
	}
}
#endif