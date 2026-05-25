// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Systems/SystemicGameplayTags.h"

#include "Systems/Reactions/SystemicReaction.h"

#include "SystemicReactionAddTraits.generated.h"

/**
 *	USystemicReactionAddTraits Class Definition.
 *		This class reacts to an event by applying traits to the event subject.
 */
UCLASS()
class JOYCORE_API USystemicReactionAddTraits : public USystemicReaction
{
	GENERATED_BODY()

protected:
	// Traits to add to the event target.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reaction|Config", meta=(GameplayTagFilter="System.Trait"))
	FGameplayTagContainer TraitTags = FGameplayTagContainer();

public:
	// USystemicReaction.	
	virtual bool Execute(const FSystemicEvent& Event, FSystemicRuleContext& Context, FSystemicTrace& Trace) override;
	// ~USystemicReaction.
};