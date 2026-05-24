// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Reactions/SystemicReaction.h"

#include "Systems/Events/SystemicEvent.h"

#include "SystemicReactionEmitEvent.generated.h"

/**
 *	USystemicReactionEmitEvent Class Definition.
 *		This class reacts to an event by invoking a chained event.
 */
UCLASS()
class JOYCORE_API USystemicReactionEmitEvent : public USystemicReaction
{
	GENERATED_BODY()

protected:
	// The event tag for the reaction event.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config", meta=(GameplayTagFilter=TAG_System_Event))
	FGameplayTag ReactionEventTag;

	// The event priority for the created event.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config", meta=(GameplayTagFilter=TAG_System_Event_Priority))
	FGameplayTag ReactionEventPriorityTag = TAG_System_Event_Priority_Default;

	// The subject of the reaction event.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config")
	ESystemicEventSubject ReactionEventSubject = ESystemicEventSubject::Target;
	
	// The event data struct for the reaction's event.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config", meta = (BaseStruct = "/Script/JoyCore.SystemicEventData"))
	FInstancedStruct ReactionEventDataStruct = FInstancedStruct(FSystemicEventData::StaticStruct());
	
public:
	/**
	 * Blueprint native event to create and fill out an event structure; base implementation emits a generic event with ::ReactionEventTag and the event subject.
	 * @param Event Triggering event.
	 * @param Context Cached data from the rule context.
	 * @param Trace Trace information for debugging and logging.
	 * @param ReactionEventOut The created reaction event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Systems|Reactions", meta = (ForceAsFunction))
	void CreateReactionEvent(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace, FSystemicEvent& ReactionEventOut);
	
	// USystemicReaction.
	virtual bool Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) override;
	// ~USystemicReaction.
};
