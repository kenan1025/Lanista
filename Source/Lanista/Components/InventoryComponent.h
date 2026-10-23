// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemDefinition.h"
#include "Items/Fragments/ItemInventoryFragment.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	/** References the item definition occupying this slot. */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UItemDefinition> ItemDef;
	/** Stores the number of items in this slot. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
	/** Initializes an empty slot with no item definition and zero quantity. */
	FInventorySlot(): ItemDef(nullptr), Quantity(0){}
	/** Returns whether the item definition is missing or the quantity is nonpositive. */
	bool IsEmpty() const {return ItemDef == nullptr || Quantity <=0;}
};
/** Notifies listeners that inventory slot contents have changed. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Enables ticking for the inventory component. */
	UInventoryComponent();
	/** Defines the number of inventory slots allocated at play start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 InventorySize = 20;
	/** Broadcasts after an item is added or an occupied slot is modified by removal. */
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	/** Adds a positive amount to the first eligible stack or empty slot without clamping the resulting stack size. */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool AddItem(UItemDefinition * Item, int32 Amount =1);
	/** Validates the slot, amount, owner, and world before removing items without currently spawning a world pickup. */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool DropItem(int32 SlotIndex, int32 Amount);
	/** Attempts to drop the requested amount from the first nonempty slot and returns that attempt's result. */
	bool DropFirstItem(int32 Amount = 1);
	/** Subtracts the supplied amount from a valid occupied slot, clears nonpositive quantities, and broadcasts an update. */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RemoveItem(int32 SlotIndex, int32 Amount = 1);
	/** Retains a legacy fragment reference that AddItem does not use for its per-item fragment lookup. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItemInventoryFragment* ItemInventoryFragment;

protected:
	/** Allocates InventorySize inventory slots and four armor slots. */
	virtual void BeginPlay() override;
	/** Stores the item stacks searched and modified by inventory operations. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;
	
	/** Stores four armor slots allocated at play start but not used by the current inventory operations. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Armors;

public:	
	/** Forwards the frame update to the base component without additional inventory logic. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
