// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"





// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Owner name is %s"), *GetOwner()->GetActorNameOrLabel());;

	Stamina = MaxStamina;
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateStamina(DeltaTime);
}




void UStaminaComponent::DrainStamina(float value)
{
	Stamina = FMath::Max(Stamina - value, 0.0f);
	OnStaminaMeterUpdated.Broadcast(Stamina / MaxStamina);
}

void UStaminaComponent::UpdateStamina(float DeltaTime)
{
	// are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (!bRecovering)
	{
		if (Stamina == 0.0f)
		{
			// raise the recovering flag
			bRecovering = true;
			OnRecoveryStateChanged.Broadcast(true);
		}
	}
	else
	{
		// recover stamina
		Stamina = FMath::Min(Stamina + StaminaRegen * DeltaTime, MaxStamina);


		if (Stamina == MaxStamina)
		{
			// lower the recovering flag
			bRecovering = false;
			OnRecoveryStateChanged.Broadcast(false);
		}
	}

	// broadcast the sprint meter updated delegate
	OnStaminaMeterUpdated.Broadcast(Stamina / MaxStamina);
}
