// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Engine/World.h"
#include "Items/WorldItem.h"
#include "Items/Fragments/ItemInventoryFragment.h"

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
	const UItemInventoryFragment* InventoryFragment =
		Cast<UItemInventoryFragment>(
			Item->FindFragmentByClass(UItemInventoryFragment::StaticClass()
			)
		);
	if (InventoryFragment)
	{
		
		
			if (InventoryFragment->bIsStackable)
			{
				for (FInventorySlot& Slot : Slots)
				{
					if (Slot.ItemDef == Item && Slot.Quantity < InventoryFragment->MaxStackSize)
					{
						
						Slot.Quantity +=Amount;
						OnInventoryUpdated.Broadcast();
				 
						return true;
					}
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

bool UInventoryComponent::DropFirstItem(int32 Amount)
{
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		if (!Slots[SlotIndex].IsEmpty())
		{
			return DropItem(SlotIndex, Amount);
		}
	}
	return false;
}

bool UInventoryComponent::DropItem(int32 SlotIndex, int32 Amount)
{
	if (!Slots.IsValidIndex(SlotIndex) || Slots[SlotIndex].IsEmpty() ||
		Amount <= 0 || Amount > Slots[SlotIndex].Quantity || !GetOwner() || !GetWorld())
	{
		return false;
	}

	UItemDefinition* Item = Slots[SlotIndex].ItemDef;
	// UClass* WorldItemClass = Item->WorldItem.LoadSynchronous();
	// if (!WorldItemClass)
	// {
	// 	return false;
	// }

	const FVector Location = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 150.0f;
	const FTransform Transform(FRotator::ZeroRotator, Location);
	// AWorldItem* DroppedItem = GetWorld()->SpawnActorDeferred<AWorldItem>(WorldItemClass, Transform,
	// 	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	// if (!DroppedItem)
	// {
	// 	return false;
	// }

	// Construction scripts and BeginPlay must see the dropped stack, not Blueprint defaults.
	// DroppedItem->ItemData = Item;
	// DroppedItem->Quantity = Amount;
	// DroppedItem->FinishSpawning(Transform, false, nullptr, ESpawnActorScaleMethod::MultiplyWithRoot);
	// if (!IsValid(DroppedItem))
	// {
	// 	return false;
	// }

	RemoveItem(SlotIndex, Amount);
	return true;
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

