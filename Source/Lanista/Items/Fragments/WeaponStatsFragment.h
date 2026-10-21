// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "WeaponStatsFragment.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class LANISTA_API UWeaponStatsFragment : public UItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CriticalChance = 0.1f;
};
