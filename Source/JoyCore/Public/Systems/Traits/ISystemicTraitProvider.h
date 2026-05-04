// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayTags.h"

#include "ISystemicTraitProvider.generated.h"

UINTERFACE(Blueprintable, MinimalAPI, Category="Game|Systems")
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

public:
	/**
	 *	Adds a trait tag to the component.
	 *	@param TraitTag The tag to add.
	 *	@return Returns true if it was successfully added.
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool AddTrait(const FGameplayTag& TraitTag) = 0;

	/**
	 *	Adds trait tags to the component.
	 *	@param TraitTagContainer Container of tags to add.
	 *	@return Returns true if any passed tag was successfully added.
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool AddTraits(const FGameplayTagContainer& TraitTagContainer) = 0;

	/**
	 *	Removes a trait tag from the component.
	 *	@param TraitTag The tag to remove.
	 *	@return Returns true if it was successfully removed.
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool RemoveTrait(const FGameplayTag& TraitTag) = 0;

	/**
	 *	Removed all passed-in container's tags from the current trait tags.
	 *	@param TraitTagContainer Container of tags to remove.
	 *	@return Returns true if any tags were removed.
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool RemoveTraits(const FGameplayTagContainer& TraitTagContainer) = 0;

	
	/**
	 *	Check if the provider has a specific trait tag.
	 *	@param TraitTag Trait tag to check for.
	 *	@returns True if the provider has the specified trait tag, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool HasTrait(const FGameplayTag& TraitTag) const
	{
		return(GetTraits().HasTag(TraitTag));
	}

	/**
	 *	Check if the provider has all specified trait tags.
	 *	@param TraitTagContainer Container of trait tags to check for.
	 *	@returns True if the provider has all the specified trait tags, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool HasTraits(const FGameplayTagContainer& TraitTagContainer) const
	{
		return(GetTraits().HasAll(TraitTagContainer));
	}

	/**
	 *	Check if the provider has the queried tag state.
	 *	@param TraitTagQuery Query to run against the provider's trait tags.
	 *	@returns True if the query passes, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual bool QueryTraits(const FGameplayTagQuery& TraitTagQuery) const
	{
		return(GetTraits().MatchesQuery((TraitTagQuery)));
	}

	/**
	 *	Get the tag container of the provider.
	 *	@returns FGameplayTagContainer of the provider.
	 */
	UFUNCTION(Category="Game|Systems|Traits", meta=(GameplayTagFilter="System.Trait"))
	virtual const FGameplayTagContainer& GetTraits() const = 0;
};
