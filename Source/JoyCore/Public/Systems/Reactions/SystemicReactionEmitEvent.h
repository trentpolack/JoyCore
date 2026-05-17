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
	// The event to emit on reaction.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config")
	TInstancedStruct<FSystemicEvent> ReactionEvent;

	// The subject of the reaction event.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config")
	ESystemicEventSubject EventSubject = ESystemicEventSubject::Target;
	
public:
	/**
	 * Populates the event data for the reaction's event.
	 * @param Event The triggering event.
	 * @param Context The rule context populated with event data.
	 * @param EventDataOut The populated event data structure.
	 * @param EventDataStruct The event data struct type for the event (FSystemicEventData if not provided).
	 */
	UFUNCTION(Blueprintable, BlueprintCallable, Category="Game|Systems|Reactions")
	virtual void PopulateEventData(const FSystemicEvent& Event, FSystemicRuleContext& Context, TInstancedStruct<FSystemicEventData>& EventDataOut, const UScriptStruct* EventDataStruct = nullptr);
	
	// USystemicReaction.	
	virtual bool Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) override;
	// ~USystemicReaction.
};
