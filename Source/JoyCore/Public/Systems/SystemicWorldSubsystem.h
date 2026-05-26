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
struct FSystemicRuleContext;

/**
 *	FSystemicRuleRuntimeData Structure.
 *		Contains any runtime data associated with the linked USystemicRule asset.
 *		NOTE (trent, 5/26/26): Along with rule mappings, this doesn't seem necessary to expose to Blueprint.
 */
USTRUCT()
struct FSystemicRuleRuntimeData
{
	GENERATED_BODY()
	
	// Rule asset pointer.
	UPROPERTY(Transient)
	TSoftObjectPtr<USystemicRule> Rule = nullptr;
	
	// Current cooldown on this rule.
	UPROPERTY(Transient)
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
 *		NOTE (trent, 5/26/26): I can't think of any reason why this would need to be exposed to Blueprint unless the use case is a whole cloth rewrite of functionality in Blueprint (which should never be necessary).
 */
USTRUCT()
struct FSystemicMappedRules
{
	GENERATED_BODY()
	
	UPROPERTY(Transient, meta=(GameplayTagFilter="System.Event"))
	FGameplayTag EventTag;

	UPROPERTY(Transient)
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
	// List of rules on cooldown.
	TArray<FSystemicRuleRuntimeData*> RulesOnCooldown;

protected:
	// Map of Rule Assets that are triggered by a given event tag (not directly exposed to Blueprint).
	UPROPERTY(Transient, meta=(GameplayTagFilter="System.Event"))
	TMap<FGameplayTag, FSystemicMappedRules> RuleMap;

	// Map of event tags to event data payload structures (not directly exposed to Blueprint).
	UPROPERTY(Transient, meta=(GameplayTagFilter="System.Event", BaseStruct="/Script/JoyCore.SystemicEventData"))
	TMap<FGameplayTag, const UScriptStruct*> EventDataStructureMap;

	// Queue of events to process during Tick; only used if the JoyCore.Systems.ProcessEventsImmediately console variable is set to 0 (default).
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Events|Transient")
	TArray<FSystemicEvent> EventQueue;

	// Number of events to process during ::Tick; process all events every ::Tick if set to 0.
	UPROPERTY(BlueprintReadOnly, Config, EditAnywhere, Category="SystemicWorld|Config")
	int32 EventProcessCountPerTick = 10;

protected:
	/**
	 * Method to fill out the EventDataStructureMap with FSystemicEvent-based payload types for each event tag.
	*/
	UFUNCTION(Category="SystemicWorld|Events")
	virtual void InitializeEventDataStructureMap();

	/**
	 * Native method to update the Event-to-EventData map.
	 * @param EventTags Gameplay tags that all map to a shared FSystemicEventData type.
	 * @param EventDataStruct The script struct representing the event data.
	 * @param bAllowOverwrite Whether to allow overwriting existing mappings (default: false).
	 * @return True if the mapping was successfully added or updated, false otherwise.
	 */
	virtual bool AddEventStructMappings(const TArray<FGameplayTag>& EventTags, const UScriptStruct* EventDataStruct, bool bAllowOverwrite = false);
	
	/**
	 * Get the event data struct for a given event tag.
	 * @param EventTag The event tag to get the data struct for.
	 * @return The event data struct for the given event tag, or nullptr if not found (and ensure).
	 */
	UFUNCTION(BlueprintCallable, Category="SystemicWorld|Events", meta=(GameplayTagFilter="System.Event"))
	virtual const UScriptStruct* GetEventDataStructForEvent(const FGameplayTag& EventTag) const;

	/**
	 * Process a systemic event.
	 * @param Event The event to process.
	 * @param bIgnoreRuleCooldowns Whether to ignore cooldowns when finding matching rules (defaults to false).
	 * @returns True if the event was successfully processed, false otherwise.
	 */
	UFUNCTION(Category="Game|Systems")
	virtual bool ProcessSystemicEvent(const FSystemicEvent& Event, const bool bIgnoreRuleCooldowns = false);
	
	/**
	 * Evaluate a rule for an event by seeing if all of its conditions pass.
	 * @param Rule The rule to execute.
	 * @param Event The event to execute the rule against.
	 * @param Trace The trace instance to fill out while evaluating the rule.
	 * @param RuleContextOut The rule context to fill out while evaluating the rule.
	 * @returns True if the rule was successfully evaluated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool EvaluateRule(USystemicRule* Rule, const FSystemicEvent& Event, FSystemicTrace& Trace, FSystemicRuleContext& RuleContextOut) const;

	/**
	 * Executes reactions in the specified rule's reaction list.
	 *	NOTE (4/19/26, trent): Not entirely sure this method needs to be separated from ::EvaluateRule or if it'd be better to execute within the rule itself. Keeping ownership at the Subsystem level for now.
	 * @param Rule The reactions specified in Rule that will be executed by this method.
	 * @param Event The triggering event.
	 * @param RuleContext The rule context filled out while evaluating the rule.
	 * @param Trace The trace instance to fill out while executing the rule's reactions.
	 * @returns True if all reactions executed successfully, false if any reaction failed to execute.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool ExecuteReactions(USystemicRule* Rule, const FSystemicEvent& Event, FSystemicRuleContext& RuleContext, FSystemicTrace& Trace) const;

	/**
	 * Find rules matching the event tag.
	 * @param EventTag The tag of the event to find matching rules for.
	 * @returns An array of pointers to rules matching the event tag.
	 */
	TArray<FSystemicRuleRuntimeData*> FindMatchingRules(const FGameplayTag& EventTag);

public:
	/**
	 * Get the USystemicWorldSubsystem instance for the given world context object.
	 * @param WorldContextObject The context object to get the subsystem for.
	 * @return The SystemicWorldSubsystem instance or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	static USystemicWorldSubsystem* Get(const UObject* WorldContextObject);

	/**
	 * Blueprint-exposed method to update the Event-to-EventData map.
	 * @param EventTag Gameplay tag to map to a shared FSystemicEventData type.
	 * @param EventDataStruct The script struct representing the event data.
	 * @param bAllowOverwrite Whether to allow overwriting existing mappings (default: false).
	 * @return True if the mapping was successfully added or updated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems", meta=(GameplayTagFilter="System.Event", ReturnDisplayName="Success"))
	virtual bool AddEventStructMapping(const FGameplayTag& EventTag, UPARAM(meta=(AllowedClasses="SystemicEventData")) const UScriptStruct* EventDataStruct, bool bAllowOverwrite = false);

	/**
	 *	Make a systemic event with the proper payload type.
	 *	@param EventOut The event to populate with the proper payload type.
	 *	@param EventTag The gameplay tag representing the type of event.
	 *	@param Priority The priority tag for the event.
	 *	@param Subject The subject of the event.
	 *	@param Source The source object of the event.
	 *	@param Instigator The instigator object of the event.
	 *	@param Target The target object of the event.
	 *	@returns True if the event was successfully created, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Events", meta=(ReturnDisplayName="Succeeded"))
	virtual bool MakeSystemicEvent(FSystemicEvent& EventOut, const FGameplayTag& EventTag, const FGameplayTag& Priority, const ESystemicEventSubject Subject, UObject* Source, AActor* Instigator, UObject* Target);
	
	/**
	 *	Emit an event to the systemic world subsystem.
	 *	@param Event The event to emit.
	 *	@returns True if the event was successfully emitted.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool EmitEvent(const FSystemicEvent& Event);

	/**
	 * Static version of EmitEvent. Allows for blueprint exposure without needing to get a reference to the subsystem.
	 * @param WorldContextObj World context object pointer.
	 * @param Event Event to emit.
	 * @returns True if the event was successfully emitted.
	 */
	static bool EmitEvent(const UObject* WorldContextObj, const FSystemicEvent& Event);

	/**
	 * Register a Rule Asset with the subsystem (and add it to the rule cache).
	 * @param RuleIn The Rule Asset to register.
	 * @returns Returns true if the registration was successful.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems")
	virtual bool RegisterRule(USystemicRule* RuleIn);
	
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
