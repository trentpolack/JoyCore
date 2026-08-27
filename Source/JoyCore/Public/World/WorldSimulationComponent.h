// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "WorldSimulationComponent.generated.h"

// Declarations.
class AEnvironmentManager;

// Log declaration.
DECLARE_LOG_CATEGORY_EXTERN(LogJoyCoreWorldSimulation, Log, All);

/**
 * UWorldSimulationComponent Class Definition.
 *	Manages the world state and associated game sim logic; intended to be attached to the Game Mode.
 */
UCLASS(Blueprintable, Category="Game|World", ClassGroup=(JoyCore), meta = (BlueprintSpawnableComponent))
class JOYCORE_API UWorldSimulationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Reference to an in-world Environment Manager; assumed to be a user-created world actor.
	UPROPERTY(BlueprintReadOnly, Category="Environment")
	TObjectPtr<AEnvironmentManager> EnvironmentManager = nullptr;

	// Source of the game world's current Time of Day.
	UPROPERTY(Transient, AdvancedDisplay, meta=(ForceUnits="Hours"))
	float TimeOfDay = 0.0f;
	
	/** 
	 * Update Time of Day and sync the Environment Manager with it.
	 * @param DeltaTime The time elapsed since the last update (in seconds).
	 * @return The game world's current Time of Day.
	 */
	UFUNCTION(Category="Environment")
	virtual float UpdateTimeOfDay(UPARAM(meta=(ForceUnits="Seconds")) float DeltaSeconds);

	/** 
	 * Update Time of Day and sync the Environment Manager with it.
	 * @param TimeOfDayIn The time elapsed since the last update (in seconds).
	 */
	UFUNCTION(Category="Environment")
	void SetTimeOfDay(UPARAM(meta=(DisplayName="Time of Day", ForceUnits="Hours")) float TimeOfDayIn);

public:
	// Constructor.
	UWorldSimulationComponent();

	// The number of minutes per second that the game world's Time of Day advances; capped to a sane ceiling of 1000 minutes/second (only in the UI).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(ForceUnits="Minutes", UIMin=0.0f, UIMax=1000.0f))
	float TimeOfDayMinutesPerSecond = 5.0f;

	// The time threshold that marks the beginning of the dawn time phase (must be less than day, dusk, and night).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(ForceUnits="Hours", UIMin=0.0f, UIMax=24.0f))
	float DawnHourThreshold = 6.0f;
	// The time threshold that marks the beginning of the day time phase (must be greater than dawn and less than dusk and night).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(ForceUnits="Hours", UIMin=0.0f, UIMax=24.0f))
	float DayHourThreshold = 8.0f;
	// The time threshold that marks the beginning of the dusk world state (must be greater than Dawn and Day and less than night).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(ForceUnits="Hours", UIMin=0.0f, UIMax=24.0f))
	float DuskHourThreshold = 18.0f;
	// The time threshold that marks the beginning of the night world state (must be greater than dusk, day, and dawn).
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(ForceUnits="Hours",UIMin=0.0f, UIMax=24.0f))
	float NightHourThreshold = 20.0f;

	// Source of the game world's current Time of Day.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Environment|TimeOfDay", meta=(DisplayName="Initial Time of Day", ForceUnits="Hours", UIMin=0.0f, UIMax=24.0f))
	float TimeOfDayInitial = 10.0f;
	
	/** 
	 * Accessor for the game world's current Time of Day ([0.0, 24.0]).
	 * @return The game world's current Time of Day.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Environment", meta=(ForceUnits="Hours"))
	float GetTimeOfDay() const;
	
	/**
	 *	Static method to update the Time of Day based on the passed-in time of day and delta time.
	 * @param TimeOfDayIn The current time of day.
	 * @param DeltaSeconds The delta time.
	 * @param MinutesPerSecondIn The number of minutes to advance the simulation per second.
	 * @return The updated time of day.
	 */
	UFUNCTION(BlueprintCallable, Category="Environment", meta=(ForceUnits="Hours"))
	static float CalculateTimeOfDay(UPARAM(meta=(DisplayName="Time of Day", ForceUnits="Hours")) float TimeOfDayIn, UPARAM(meta=(ForceUnits="Seconds")) float DeltaSeconds, UPARAM(meta=(DisplayName="Minutes Per Second", ForceUnits="Minutes")) float MinutesPerSecondIn = 10.0f);
	
	// UActorComponent.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ~UActorComponent.
};