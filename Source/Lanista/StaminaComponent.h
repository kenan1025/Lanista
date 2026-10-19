// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateStaminaMeterDelegate, float, Percentage);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaStateChangedDelegate, bool, bSprinting);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();


public:	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	
	
	/** If true, we're recovering stamina */
	bool bRecovering = false;
	bool bSprinting;
	UPROPERTY(EditAnywhere)
	float MaxStamina = 100.0f;
	
	UPROPERTY(VisibleAnywhere)
	float Stamina;
	
	UPROPERTY(EditAnywhere)
	float StaminaRegen = 10.0f;
	
	UFUNCTION()
	void OnSprintChanged(bool bIsSprinting);
	
	
	

	/** Delegate called when the stamina meter should be updated */
	FUpdateStaminaMeterDelegate OnStaminaMeterUpdated;

	
	
	void DrainStamina(float value);

	
	void UpdateStamina(float DeltaTime);
	

	
	
		
};
