// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GameplayTags.h"

#include "SystemicWorldSubsystem.generated.h"

// Declarations.
class USystemicRule;

struct FSystemicEvent;
struct FSystemicTrace;

// Log declaration.
DECLARE_LOG_CATEGORY_EXTERN(LogJoyCoreSystems, Log, All);

/**
 *	FSystemicRuleRuntimeData Structure.
 *		Contains any runtime data associated with the linked USystemicRule asset.
 */
USTRUCT()
struct FSystemicRuleRuntimeData
{
	GENERATED_BODY()
	
	// Rule asset pointer.
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	TSoftObjectPtr<USystemicRule> Rule = nullptr;
	
	// Current cooldown on this rule.
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	float Cooldown = -1.0f;

	/**
	 * FSystemicRuleRuntimeData Constructor.
	 */
	FSystemicRuleRuntimeData()
	{	}

	/**
	 * FSystemicRuleRuntimeData Constructor.
	 * @param RuleIn Rule asset object pointer.
	 * @param CooldownIn Cooldown value for this rule.
	 */
	FSystemicRuleRuntimeData(const TSoftObjectPtr<USystemicRule> RuleIn, const float CooldownIn)	
	{
		Rule = RuleIn;
		Cooldown = CooldownIn;
	}
};

/**
 *	FSystemicMappedRules Structure.
 *		Workaround for not being able to directly use an array in a UPROPERTY.
 *		Contains a GameplayTag and an array of FSystemicRuleRuntimeData (rule assets in addition to any runtime data).
 */
USTRUCT()
struct FSystemicMappedRules
{
	GENERATED_BODY()
	
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	FGameplayTag EventTag;

	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly)
	TArray<FSystemicRuleRuntimeData> Rules;
};

/**
 * USystemicWorldSubsystem Class Definition.
 *	This is the subsystem that receives events from the game sim and evaluates them with Rules and Conditions to trigger an eventual response (reaction).
 */
UCLASS(Blueprintable, Category="Game|Systems", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

private:
	// Map of Rule Assets that are triggered by a given event tag..
	TArray<FSystemicRuleRuntimeData*> RulesOnCooldown;

protected:
	// Map of Rule Assets that are triggered by a given event tag..
	UPROPERTY(Transient, Category="Runtime", VisibleInstanceOnly, meta=(GameplayTagFilter="System.Event"))
	TMap<FGameplayTag, FSystemicMappedRules> RuleMap;

	// Queue of events to process during Tick; only used if the JoyCore.Systems.ProcessEventsImmediately console variable is set to 0 (default).
	UPROPERTY(Category="Runtime", VisibleInstanceOnly)
	TArray<FSystemicEvent> EventQueue;

	// Number of events to process during ::Tick; process all events every ::Tick if set to 0.
	UPROPERTY(Config, EditAnywhere, Category="Settings")
	int32 EventProcessCountPerTick = 10;

protected:
	/**
	 * Process a systemic event.
	 * @param Event The event to process.
	 * @param bIgnoreDisabledRules Whether to ignore disabled rules when finding matching rules (defaults to true).
	 * @param bIgnoreRuleCooldowns Whether to ignore cooldowns when finding matching rules (defaults to false).
	 */
	UFUNCTION(Category="Game|Systems")
	virtual bool ProcessSystemicEvent(const FSystemicEvent& Event, const bool bIgnoreDisabledRules = true, const bool bIgnoreRuleCooldowns = false);
	
	/**
	 * Executes a rule against an event, evaluating its conditions and triggering its reaction if successful.
	 * @param Rule The rule to execute.
	 * @param Event The event to execute the rule against.
	 * @param Trace The trace instance to fill out while executing the rule.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool ExecuteRule(USystemicRule* Rule, const FSystemicEvent& Event, FSystemicTrace& Trace);

	/**
	 * Find rules matching the event tag.
	 * @param EventTag The tag of the event to find matching rules for.
	 * @param bIgnoreDisabled Whether to ignore disabled rules when finding matching rules (defaults to true).
	 * @return An array of pointers to rules matching the event tag.
	 */
	TArray<FSystemicRuleRuntimeData*> FindMatchingRules(const FGameplayTag& EventTag, const bool bIgnoreDisabled = true);

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
	virtual bool RegisterRule(USystemicRule* RuleIn, bool bForceActivateRule = true);
	
	// UTickableWorldSubsystem.
	virtual ETickableTickType GetTickableTickType() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// ~UTickableWorldSubsystem.
	
	// USubsystem.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~USubsystem.
};
