// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Fragments/ItemFragment.h"
#include "ItemRepresentationFragment.generated.h"

/**
 * 
 */
UCLASS()
class LANISTA_API UItemRepresentationFragment : public UItemFragment
{
	GENERATED_BODY()
public:
	/** References the item's configured static mesh without creating a mesh component automatically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Mesh")
	TObjectPtr<UStaticMesh> ItemMesh;
	/** Stores a drop-permission setting that the current inventory drop implementation does not enforce. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Inventory")
	bool bCanBeDropped = true;
};
