// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** Carries the owner's alive state when health reaches zero. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHealthStateChangedDelegate, bool, bIsAlive);
/** Carries the current health fraction after damage is processed. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHealthUpdatedDelegate, float, Percentage);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Enables ticking for the health component. */
	UHealthComponent();

protected:
	/** Initializes health to MaxHealth and binds the owner's damage event. */
	virtual void BeginPlay() override;

public:	
	/** Forwards the frame update to the base component without additional health logic. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/** Defines the initial health and upper bound used when applying damage. */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float MaxHealth = 100.0f;
	/** Stores the current health after initialization and armor-adjusted damage. */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float Health;
	/** Controls the armor damage multiplier through the formula K / (K + MaxArmor). */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float K=100.0f;
	/** Determines whether incoming damage is processed and becomes false at zero health. */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	bool bIsAlive = true;
	/** Stores the armor value used to reduce incoming damage. */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Armor")
	float MaxArmor= 0.0f;
	/** Broadcasts the false alive state when processed damage leaves health at zero. */
	UPROPERTY(BlueprintAssignable,Category="Health")
	FSHealthStateChangedDelegate OnHealthChanged;
	/** Broadcasts Health divided by MaxHealth after damage is processed while alive. */
	UPROPERTY(BlueprintAssignable,Category="Health")
	FSHealthUpdatedDelegate OnHealthUpdated;
	
	/** Applies armor-adjusted damage while alive, clamps health, and broadcasts health and death updates. */
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
