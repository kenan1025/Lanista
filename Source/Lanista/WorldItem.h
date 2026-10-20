// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "GameFramework/Actor.h"
#include "WorldItem.generated.h"

UCLASS()
class LANISTA_API AWorldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "ItemDefinition")
	UItemDefinition* ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "ItemDefinition")
	int32 Quantity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "ItemDefinition")
	FText DisplayName;

};
