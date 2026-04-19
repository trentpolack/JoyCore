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
	 *	@param Tag The tag to add.
	 */
	UFUNCTION(Category="Game|Systems")
	virtual void AddTraitTag(const FGameplayTag& Tag) = 0;

	/**
	 *	Adds trait tags to the component.
	 *	@param TagContainer Container of tags to add.
	 */
	UFUNCTION(Category="Game|Systems")
	virtual void AddTraitTags(const FGameplayTagContainer& TagContainer) = 0;

	/**
	 *	Check if the provider has a specific trait tag.
	 *	@param Tag Trait tag to check for.
	 *	@returns True if the provider has the specified trait tag, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems")
	virtual bool HasTraitTag(const FGameplayTag& Tag) const
	{
		return(GetTraitTags().HasTag(Tag));
	}

	/**
	 *	Check if the provider has all specified trait tags.
	 *	@param TagContainer Container of trait tags to check for.
	 *	@returns True if the provider has all the specified trait tags, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems")
	virtual bool HasTraits(const FGameplayTagContainer& TagContainer) const
	{
		return(GetTraitTags().HasAll(TagContainer));
	}

	/**
	 *	Check if the provider has the queried tag state.
	 *	@param TagQuery Query to run against the provider's trait tags.
	 *	@returns True if the query passes, false otherwise. 
	 */
	UFUNCTION(Category="Game|Systems")
	virtual bool QueryTraits(const FGameplayTagQuery& TagQuery) const
	{
		return(GetTraitTags().MatchesQuery((TagQuery)));
	}

	/**
	 *	Get the tag container of the provider.
	 *	@returns FGameplayTagContainer of the provider.
	 */
	UFUNCTION(Category="Game|Systems")
	virtual const FGameplayTagContainer& GetTraitTags() const = 0;
};
