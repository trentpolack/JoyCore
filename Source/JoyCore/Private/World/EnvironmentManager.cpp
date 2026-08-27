// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "World/EnvironmentManager.h"
#include "World/WorldSimulationComponent.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/WindDirectionalSourceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EnvironmentManager)

// Constructor.
AEnvironmentManager::AEnvironmentManager(const FObjectInitializer& Init)
: Super(Init)
{
	// This actor only ticks in editor (if configured); otherwise its time of day should be managed by the game sim.
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
	SunMoonRootComponent->SetWorldRotation(FRotator::ZeroRotator);
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
	SunLightComponent->SetTransmission(true);
	SunLightComponent->SetCastDeepShadow(true);
	SunLightComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	SunLightComponent->bCastCloudShadows = true;
	SunLightComponent->bCastShadowsOnClouds = true;
	SunLightComponent->bCastShadowsOnAtmosphere = true;
	SunLightComponent->bPerPixelAtmosphereTransmittance = true;
	SunLightComponent->CloudShadowExtent = 25.0f;
	
	// Configure other Moon default properties (and offset by 180 degrees).
	MoonLightComponent->SetAtmosphereSunLight(true);
	MoonLightComponent->SetAtmosphereSunLightIndex(1);
	MoonLightComponent->SetForwardShadingPriority(0);
	MoonLightComponent->SetIntensity(0.25f);
	MoonLightComponent->SetUseTemperature(true);
	MoonLightComponent->SetTemperature(9000.0f);
	MoonLightComponent->SetTransmission(true);
	MoonLightComponent->SetCastDeepShadow(true);
	MoonLightComponent->SetWorldRotation(FRotator(180.0f, 0.0f, 0.0f));
	MoonLightComponent->bCastCloudShadows = true;
	MoonLightComponent->bCastShadowsOnAtmosphere = true;
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

// Update the environment based on the specified time of day.
void AEnvironmentManager::UpdateEnvironment()
{
	// Update the sun and moon orientation for the current time of day.
	UpdateSunAndMoon();
}

// Update the sun and moon to represent the specified time of day.
void AEnvironmentManager::UpdateSunAndMoon()
{
	if(!IsValid(SunMoonRootComponent))
	{
		// Invalid state.
		return;
	}
	
	// Calculate the pitch of the Sun and Moon with optional user-specified pitch offset.
	const float Pitch = FMath::UnwindDegrees((-((TimeOfDay - SunRiseHour)/24.0f)*360.0f) + SunMoonPitchOffset);
	
	// Update the orientation of the root component for the Sun and Moon directional lights.
	SunMoonRootComponent->SetRelativeRotation(FRotator(Pitch, SunMoonYaw, 0.0f));
}

// Method to set the Time of Day to update the environment visuals.
void AEnvironmentManager::SetTimeOfDay(float TimeOfDayIn)
{
	// Set the environment's updated Time of Day.
	TimeOfDay = TimeOfDayIn;
	
	// Update the environment state.
	UpdateEnvironment();
}

// Begin play.
void AEnvironmentManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the initial time of day.
	SetTimeOfDay(TimeOfDay);
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
		SetTimeOfDay(UWorldSimulationComponent::CalculateTimeOfDay(TimeOfDay, DeltaSeconds, TimeOfDayMinutesPerSecond_Editor));
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