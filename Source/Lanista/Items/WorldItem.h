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
	/** Enables ticking for the world item actor. */
	AWorldItem();

protected:
	/** Forwards play initialization to the base actor without additional item setup. */
	virtual void BeginPlay() override;

public:	
	/** Forwards the frame update to the base actor without additional item logic. */
	virtual void Tick(float DeltaTime) override;
	/** References the item definition transferred to inventory when this actor is picked up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "ItemDefinition", meta=(ExposeOnSpawn="true"))
	TObjectPtr<UItemDefinition> ItemData;
	/** Stores the item quantity transferred to inventory during pickup. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "ItemDefinition", meta=(ExposeOnSpawn="true", ClampMin="1"))
	int32 Quantity = 1;
	

};
