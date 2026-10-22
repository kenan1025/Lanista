// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	UE_LOG(LogTemp,Warning,TEXT("Max health is %f and health is %f"),MaxHealth,Health);

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsAlive)
	{
		
		float Armor = 50.0f;
		float DamageTaken = Damage * (K/(K+Armor));
		Health = FMath::Clamp(Health - DamageTaken, 0.0f, 100.0f);
		OnHealthUpdated.Broadcast(Health/MaxHealth);
		if (Health <=0.0f)
		{
			bIsAlive = false;
			OnHealthChanged.Broadcast(bIsAlive);
		}
	}
}

