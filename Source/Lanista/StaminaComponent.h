// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateStaminaMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaStateChangedDelegate, bool, bSprinting);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** If true, we're sprinting */
	bool bSprinting = false;
	/** If true, we're recovering stamina */
	bool bRecovering = false;
	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;
	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;
	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 3.0f;
	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float SprintSpeed = 600.0f;
	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;
	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;
	/** Sprint tick timer */
	FTimerHandle SprintTimer;
	

	/** Delegate called when the sprint meter should be updated */
	FUpdateStaminaMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FStaminaStateChangedDelegate OnSprintStateChanged;
	
	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoEndSprint();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();
	UFUNCTION(BlueprintCallable)
	void CalculateStamina(AActor* TargetActor, float value);
	
	
		
};
