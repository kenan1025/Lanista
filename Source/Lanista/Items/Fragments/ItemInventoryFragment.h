// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemInventoryFragment.generated.h"

/**
 * 
 */
UCLASS()
class LANISTA_API UItemInventoryFragment : public UItemFragment
{
	GENERATED_BODY()
public:
	/** Allows AddItem to search for an existing matching stack before using an empty slot. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bIsStackable = true;
	/** Defines the quantity threshold below which AddItem may select an existing stack, not a cap on the resulting quantity. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (EditCondition = "bIsStackable"))
	int MaxStackSize = 99;
};
