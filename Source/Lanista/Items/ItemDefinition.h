// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Fragments/ItemFragment.h"

#include "ItemDefinition.generated.h"


/**
 * 
 */


UCLASS(BlueprintType, Blueprintable, Const)
class LANISTA_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Stores the player-facing name of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText ItemName;
	/** Stores the player-facing description of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText ItemDescription;
	/** References the item's display icon without loading it automatically. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TSoftObjectPtr<UTexture2D> Icon;

	/** Stores the instanced fragments that supply this item's configurable data. */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="Fragments Array")
	TArray<TObjectPtr<UItemFragment>> Fragments;
	/** Returns the first nonnull fragment matching the requested class or a subclass, or nullptr if none matches. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "FragmentClass"))
	const UItemFragment* FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass);
};
