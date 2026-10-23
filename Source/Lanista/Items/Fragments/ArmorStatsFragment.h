// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Fragments/ItemFragment.h"
#include "ArmorStatsFragment.generated.h"

/**
 * 
 */
UCLASS()
class LANISTA_API UArmorStatsFragment : public UItemFragment
{
	GENERATED_BODY()
	
public:
	/** Stores the item's configured base armor value without applying it to health automatically. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseArmor = 20.0f;
};
