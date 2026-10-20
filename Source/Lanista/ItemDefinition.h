// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

/**
 * 
 */
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
};
