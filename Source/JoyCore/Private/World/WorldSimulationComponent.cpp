// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "World/WorldSimulationComponent.h"

#include "EngineUtils.h"

#include "World/EnvironmentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WorldSimulationComponent)

// Define the log category for the JoyCore systems logic.
DEFINE_LOG_CATEGORY(LogJoyCoreWorldSimulation);

UWorldSimulationComponent::UWorldSimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UWorldSimulationComponent::UpdateTimeOfDay(float DeltaTime)
{
	// Advance the time of day with the passed-in delta time and wrap around back to 0.0 if the result exceeds 24.0; using the set method to keep responding logic in one place.
	SetTimeOfDay(FMath::Wrap<float>(TimeOfDay + (DeltaTime*TimeOfDayMinutesPerSecond)/60.0f, 0.0f, 24.0f));
	
	return(GetTimeOfDay());
}

void UWorldSimulationComponent::SetTimeOfDay(float TimeOfDayIn)
{
	TimeOfDay = TimeOfDayIn;
	
	if(IsValid(EnvironmentManager))
	{
		// Synchronize the Environment Manager.
		EnvironmentManager->SetTimeOfDay(TimeOfDay);
	}
}

float UWorldSimulationComponent::GetTimeOfDay() const
{
	// Return the game world's current Time of Day.
	return TimeOfDay;
}

void UWorldSimulationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// If not already set, find an EnvironmentManager instance in the world.
	if(!IsValid(EnvironmentManager))
	{
		for(TActorIterator<AEnvironmentManager> It(GetWorld()); It; ++It)
		{
			// If the EnvironmentManager is valid, assign it to the local reference and break out.
			AEnvironmentManager* pEnvironmentManager = *It;
			if(IsValid(pEnvironmentManager))
			{
				EnvironmentManager = pEnvironmentManager;
				break;
			}
		}

		// Log a warning if unavailable.
		if(!IsValid(EnvironmentManager))
		{
			UE_LOG(LogJoyCoreWorldSimulation, Warning, TEXT("%hs was unable to find a valid EnvironmentManager in the world."), __FUNCTION__);
		}
	}
	
	// Set the initial Time of Day.
	SetTimeOfDay(TimeOfDayInitial);
}

void UWorldSimulationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void UWorldSimulationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Update the time of day.
	UpdateTimeOfDay(DeltaTime);
}
