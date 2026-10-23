// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemEquipmentFragment.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	/** Identifies an item with no assigned equipment slot. */
	None,
	/** Identifies the head equipment slot. */
	Head,
	/** Identifies the chest equipment slot. */
	Chest,
	/** Identifies the legs equipment slot. */
	Legs,
	/** Identifies the feet equipment slot. */
	Feet
};
/**
 * 
 */
UCLASS()
class LANISTA_API UItemEquipmentFragment : public UItemFragment
{
	GENERATED_BODY()
public:
	/** Marks this item as equipment and enables editing its slot and mesh settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	bool bIsEquipment = false;
	/** Stores the equipment slot assigned to this item. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "bIsEquipment"))
	EEquipmentSlot Slot = EEquipmentSlot::None;

	/** References the skeletal mesh configured to represent this equipment without loading or attaching it automatically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "bIsEquipment"))
	TSoftObjectPtr<USkeletalMesh> EquipmentMesh;
};
