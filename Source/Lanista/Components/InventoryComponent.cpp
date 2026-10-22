// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Items/WorldItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UInventoryComponent::AddItem(UItemDefinition* Item, int32 Amount)
{
	if (!Item || Amount <= 0)
	{
		return false;
	}
	if (Item->bIsStackable)
	{
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.ItemDef == Item && Slot.Quantity < Item->MaxStackSize)
			{
				Slot.Quantity +=Amount;
				OnInventoryUpdated.Broadcast();
				 
				return true;
			}
		}
	}
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.IsEmpty())
		{
			Slot.ItemDef = Item;
			Slot.Quantity = Amount;
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	return false;
}

void UInventoryComponent::RemoveItem(int32 SlotIndex, int32 Amount)
{
	if (Slots.IsValidIndex(SlotIndex) && !Slots[SlotIndex].IsEmpty())
	{
		Slots[SlotIndex].Quantity -= Amount;
		if (Slots[SlotIndex].Quantity <= 0)
		{
			Slots[SlotIndex] = FInventorySlot();
		}
		OnInventoryUpdated.Broadcast();
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Slots.SetNum(InventorySize);
	Armors.SetNum(4);
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

