// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SystemicGameplayComponentBase.generated.h"

// Declarations.
class ISystemicTraitProvider;

/**
 *	USystemicGameplayComponentBase Class.
 *		Abstract systemic gameplay component base class to provide common functionality for all children.
 */
UCLASS(Abstract, Category="Game|Systems|Components", ClassGroup=(JoyCore), Config=JoyCore)
class JOYCORE_API USystemicGameplayComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 *	USystemicGameplayComponentBase Constructor.
	 */
	USystemicGameplayComponentBase();

protected:
	// Instance of the Trait Provider interface.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, AdvancedDisplay, Category="Traits|Transient")
	TScriptInterface<ISystemicTraitProvider> TraitProvider = nullptr;
	
public:
	/** 
	 * Get the Trait Provider interface.
	 * @returns The Trait Provider interface.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Systems|Traits")
	TScriptInterface<ISystemicTraitProvider> GetTraitProvider() const;
	
	// UActorComponent.
	virtual void BeginPlay() override;
	// ~UActorComponent.
};