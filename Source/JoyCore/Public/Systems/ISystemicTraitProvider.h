// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayTags.h"

#include "ISystemicTraitProvider.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, ClassGroup=(JoyCore))
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
	 *	ISystemicTraitProvider.
	 */
	
	/**
	 *	Check if the provider has a specific trait tag.
	 *	@param Tag Trait tag to check for.
	 *	@returns True if the provider has the specified trait tag, false otherwise. 
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual bool HasTraitTag(const FGameplayTag& Tag) const
	{
		return(GetTraitTags().HasTag(Tag));
	}

	/**
	 *	Check if the provider has all specified trait tags.
	 *	@param TagContainer Container of trait tags to check for.
	 *	@returns True if the provider has all the specified trait tags, false otherwise. 
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual bool HasTraits(const FGameplayTagContainer& TagContainer) const
	{
		return(GetTraitTags().HasAll(TagContainer));
	}

	/**
	 *	Check if the provider has the queried tag state.
	 *	@param TagQuery Query to run against the provider's trait tags.
	 *	@returns True if the query passes, false otherwise. 
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual bool QueryTraits(const FGameplayTagQuery& TagQuery) const
	{
		return(GetTraitTags().MatchesQuery((TagQuery)));
	}

	/**
	 *	Get the tag container of the provider.
	 *	@returns FGameplayTagContainer of the provider.
	 */
	UFUNCTION(BlueprintCallable, Category="JoyCore|Systems|Traits")
	virtual const FGameplayTagContainer& GetTraitTags() const = 0;
};
