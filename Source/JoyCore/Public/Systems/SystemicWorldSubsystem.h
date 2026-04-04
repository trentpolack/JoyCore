// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GameplayTags.h"

#include "SystemicWorldSubsystem.generated.h"

// Declarations.
class USystemicRuleAsset;

struct FSystemicEvent;

// Log declaration.
DECLARE_LOG_CATEGORY_EXTERN(LogJoyCoreSystems, Log, All);

/**
 *	FSystemicMappedRuleAssets
 *		Workaround for not being able to directly use an array in a UPROPERTY.
 *		Contains a GameplayTag and an array of USystemicRuleAsset pointers.
 */
USTRUCT()
struct FSystemicMappedRuleAssets
{
	GENERATED_BODY()
	
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	FGameplayTag EventTag;

	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	TArray<TSoftObjectPtr<USystemicRuleAsset>> RuleAssets;
};

/**
 * USystemicWorldSubsystem Class Definition.
 *	This is the subsystem that receives events from the game sim and evaluates them with Rules and Conditions to trigger an eventual response (reaction).
 */
UCLASS(Blueprintable, Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
	// Active Rules.
	UPROPERTY(Category="Runtime", VisibleInstanceOnly)
	TArray<TObjectPtr<USystemicRuleAsset>> ActiveRules;

	// Map of Rule Assets that are triggered by a given event tag..
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly, meta=(GameplayTagFilter="System.Event"))
	TMap<FGameplayTag, FSystemicMappedRuleAssets> RuleMap;

	// Queue of events to process during Tick; only used if the JoyCore.Systems.ProcessEventsImmediately console variable is set to 0 (default).
	UPROPERTY(Category="Runtime", VisibleInstanceOnly)
	TArray<FSystemicEvent> EventQueue;

	// Number of events to process during ::Tick; process all events every ::Tick if set to 0.
	UPROPERTY(Config, EditAnywhere, Category="Settings")
	int32 EventProcessCountPerTick = 10;

protected:
	/**
	 * Process a new event.
	 * @param Event The event to process.
	 */
	UFUNCTION(Category="Game|Systems")
	virtual void ProcessSystemicEvent(const FSystemicEvent& Event);
	
	/**
	 * Executes a rule against an event, evaluating its conditions and triggering its reaction if successful.
	 * @param Rule The rule to execute.
	 * @param Event The event to execute the rule against.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual void ExecuteRule(USystemicRuleAsset* Rule, const FSystemicEvent& Event);

	/*
	 *	Find rules matching the event tag.
	 *	@param EventTag The tag of the event to find matching rules for.
	 *	@return An array of pointers to rules matching the event tag.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems", meta=(GameplayTagFilter="System.Event"))
	TArray<USystemicRuleAsset*> FindMatchingRules(FGameplayTag EventTag) const;

public:
	/**
	 *	Emit an event to the systemic world subsystem.
	 *	@param Event The event to emit.
	 */
	virtual void EmitEvent(const FSystemicEvent& Event);

	/**
	 * Static version of EmitEvent. Allows for blueprint exposure without needing to get a reference to the subsystem.
	 * @param WorldContextObj World context object pointer.
	 * @param Event Event to emit.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	static void EmitEvent(UObject* WorldContextObj, const FSystemicEvent& Event);

	/**
	 * Register a Rule Asset with the subsystem (and add it to the rule cache).
	 * @param RuleIn The Rule Asset to register.
	 * @param bForceActivateRule Optional override for rule activation state.
	 * @return Returns true if the registration was successful.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool RegisterRule(USystemicRuleAsset* RuleIn, bool bForceActivateRule = true);
	
	// UTickableWorldSubsystem.
	virtual ETickableTickType GetTickableTickType() const override;
	virtual void Tick(float DeltaTime) override;
	// ~UTickableWorldSubsystem.
	
	// USubsystem.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~USubsystem.
};
