// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Fragments/ItemFragment.h"

#include "ItemDefinition.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
 enum class EEquipmentSlot : uint8
 {
     None,
     Head,
     Chest,
     Legs,
 	 Feet
 };
UCLASS(BlueprintType, Blueprintable)
class LANISTA_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Display")
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Display")
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Gameplay")
	bool bIsStackable = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Gameplay", meta = (EditCondition = "bIsStackable"))
	int MaxStackSize = 99;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	bool bIsEquipment = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment",meta = (EditCondition = "bIsEquipment"))
        EEquipmentSlot Slot = EEquipmentSlot::None;
     
        // The Skeletal Mesh that represents this armor piece
        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment",meta = (EditCondition = "bIsEquipment"))
        TSoftObjectPtr<USkeletalMesh> EquipmentMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<FGameplayTag, float> ItemStats;
 
	// Helper function to find a stat value safely
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetStatValue(FGameplayTag StatTag) const
	{
		// If the tag exists in the map, return it. Otherwise, return 0.
		if (const float* FoundValue = ItemStats.Find(StatTag))
		{
			return *FoundValue;
		}
		return 0.0f;
	}
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<UItemFragment>> Fragments;
};
