// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "JoyCoreNativeGameplayTags.h"

#include "ISystemicTraitProvider.generated.h"

// UHT Interface definition.
UINTERFACE(Blueprintable, MinimalAPI, Category = "Game|Systems")
class USystemicTraitProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *	ISystemicTraitProvider Interface.
 */
class JOYCORE_API ISystemicTraitProvider
{
	GENERATED_BODY()

protected:
	/**
	 * Returns the name of the owner of the trait provider.
	 * @return The name of the owner.
	 */
	UFUNCTION(Category = "Game|Systems")
	virtual const FName GetOwnerName() const = 0;
	
public:
	/**
	 *	Adds a trait tag to the component.
	 *	@param TraitTag The tag to add.
	 *	@param bEmitTraitChangedEvent Whether to broadcast the OnTraitsChanged event (default: true)
	 *	@returns Returns true if it was successfully added.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool AddTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent = true) = 0;

	/**
	 *	Adds trait tags to the component.
	 *	@param TraitTags Container of tags to add.
	 *	@param bEmitTraitChangedEvent Whether to broadcast the OnTraitsChanged event (default: true)
	 *	@returns Returns a gameplay tag container with only the successfully added tags.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual const FGameplayTagContainer AddTraits(const FGameplayTagContainer& TraitTags, bool bEmitTraitChangedEvent = true) = 0;

	/**
	 *	Removes a trait tag from the component.
	 *	@param TraitTag The tag to remove.
	 *	@param bEmitTraitChangedEvent Whether to broadcast the OnTraitsChanged event (default: true).
	 *	@returns Returns true if it was successfully removed.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool RemoveTrait(const FGameplayTag& TraitTag, bool bEmitTraitChangedEvent = true) = 0;

	/**
	 *	Removed all passed-in container's tags from the current trait tags.
	 *	@param TraitTags Container of tags to remove.
	 *	@param bEmitTraitChangedEvent Whether to broadcast the OnTraitsChanged event (default: true)
	 *	@returns Returns a gameplay tag container with only the successfully removed tags.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual const FGameplayTagContainer RemoveTraits(const FGameplayTagContainer& TraitTags, bool bEmitTraitChangedEvent = true) = 0;

	/**
	 *	Add/remove traits from the current container's tags; intended for batch changes; there is no option to bypass OnTraitsChanged event for this (the option only exists for potential batch operations).
	 *	@param TraitTagsToAdd Container of tags to add.
	 *	@param TraitTagsToRemove Container of tags to remove.
	 *	@returns Returns true if any tags were successfully added/removed.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool ModifyTraits(const FGameplayTagContainer& TraitTagsToAdd, const FGameplayTagContainer& TraitTagsToRemove) = 0;
	
	/**
	 *	Check if the provider has a specific trait tag.
	 *	@param TraitTag Trait tag to check for.
	 *	@returns True if the provider has the specified trait tag, false otherwise. 
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool HasTrait(const FGameplayTag& TraitTag) const
	{
		return(GetTraits().HasTagExact(TraitTag));
	}

	/**
	 *	Check if the provider has all specified trait tags.
	 *	@param TraitTags Container of trait tags to check for.
	 *	@returns True if the provider has all the specified trait tags, false otherwise. 
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool HasTraits(const FGameplayTagContainer& TraitTags) const
	{
		return(GetTraits().HasAllExact(TraitTags));
	}

	/**
	 *	Check if the provider has the queried tag state.
	 *	@param TraitTagQuery Query to run against the provider's trait tags.
	 *	@returns True if the query passes, false otherwise. 
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual bool QueryTraits(const FGameplayTagQuery& TraitTagQuery) const
	{
		return(GetTraits().MatchesQuery((TraitTagQuery)));
	}

	/**
	 *	Get the tag container of the provider.
	 *	@returns FGameplayTagContainer of the provider.
	 */
	UFUNCTION(Category = "Game|Systems|Traits", meta = (GameplayTagFilter="System.Trait"))
	virtual const FGameplayTagContainer& GetTraits() const = 0;
};