// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHealthStateChangedDelegate, bool, bIsAlive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHealthUpdatedDelegate, float, Percentage);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Max health of character
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float MaxHealth = 100.0f;
	//Health of character
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float Health;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float K=100.0f;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	bool bIsAlive = true;
	UPROPERTY(BlueprintAssignable,Category="Health")
	FSHealthStateChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="Health")
	FSHealthUpdatedDelegate OnHealthUpdated;
	
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
