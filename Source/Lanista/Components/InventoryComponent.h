// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemDefinition.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UItemDefinition> ItemDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
	FInventorySlot(): ItemDef(nullptr), Quantity(0){}
	bool IsEmpty() const {return ItemDef == nullptr || Quantity <=0;}
};
// Delegate to notify UI of changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 InventorySize = 20;
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool AddItem(UItemDefinition * Item, int32 Amount =1);
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RemoveItem(int32 SlotIndex, int32 Amount = 1);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
