// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SystemicGameplayComponentBase.h"

#include "SystemicInteractionComponent.generated.h"

// Delegate for broadcasting successful systemic interactions.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSystemicInteractedSignature, AActor*, InstigatorActor, const FName&, InteractionName);

/**
 *	USystemicInteractionComponent Class.
 *		Actor component that exposes an interaction endpoint and emits systemic interaction events. This component adds support for interactable actors.
 */
UCLASS(Category="Game|Systems|Components", ClassGroup=(JoyCore), Config=JoyCore, meta=(BlueprintSpawnableComponent))
class JOYCORE_API USystemicInteractionComponent : public USystemicGameplayComponentBase
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicInteractionComponent Constructor.
	 */
	USystemicInteractionComponent();

protected:
	// Default interaction string constant.
	static const FName InteractionNameDefault;

	// Whether this component currently accepts and emits interactions (default: true).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction|Config")
	uint8 bEnabled = true;

public:
	// Broadcast when this component successfully receives an interaction.
	UPROPERTY(BlueprintAssignable, Category="Game|Systems|Events")
	FSystemicInteractedSignature OnInteracted;
	
	/**
	 *	Set whether this component accepts and emits interactions.
	 *	@param bEnabledIn True to enable interaction, false to disable it.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Interaction")
	void SetEnabled(bool bEnabledIn) { bEnabled = bEnabledIn; }

	/**
	 *	Check whether this component currently accepts and emits interactions.
	 *	@return True if enabled, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category="Game|Systems|Interaction")
	bool GetIsEnabled() const { return bEnabled; }
	
	/**
	 *	Attempt to interact with this component.
	 *	@param InstigatorActor Actor responsible for the interaction.
	 *	@param InteractionName Interaction name to emit; uses DefaultInteractionName when NAME_None.
	 *	@param SourceObject Object responsible for the interaction.
	 *	@return True if the interaction was accepted and emitted, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Interaction")
	virtual bool Interact(AActor* InstigatorActor, FName InteractionName = NAME_None, UObject* SourceObject = nullptr);
};