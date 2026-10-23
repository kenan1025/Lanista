// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

/** Carries the current stamina fraction for meter updates. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateStaminaMeterDelegate, float, Percentage);
/** Carries whether stamina recovery has started or finished. */
DECLARE_MULTICAST_DELEGATE_OneParam(FRecoveryStateChangedDelegate, bool);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaStateChangedDelegate, bool, bSprinting);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANISTA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Enables ticking for stamina recovery updates. */
	UStaminaComponent();


public:	
	/** Logs the owner and initializes stamina to MaxStamina. */
	virtual void BeginPlay() override;
	/** Updates stamina recovery after forwarding the frame update to the base component. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	
	
	/** Indicates the recovery phase entered at zero stamina and exited at full stamina. */
	bool bRecovering = false;
	/** Broadcasts true on recovery entry and false when stamina is fully restored. */
	FRecoveryStateChangedDelegate OnRecoveryStateChanged;
	/** Retains a legacy sprint flag that the current component implementation does not use. */
	bool bSprinting;
	/** Defines initial stamina and the upper bound for recovery. */
	UPROPERTY(EditAnywhere)
	float MaxStamina = 100.0f;
	
	/** Stores the current stamina available for draining and recovery. */
	UPROPERTY(VisibleAnywhere)
	float Stamina;
	
	/** Defines the stamina restored per second during recovery. */
	UPROPERTY(EditAnywhere)
	float StaminaRegen = 10.0f;
	
	
	
	
	

	/** Broadcasts the stamina fraction after draining and during every recovery update. */
	FUpdateStaminaMeterDelegate OnStaminaMeterUpdated;

	
	
	/** Subtracts the supplied value with a zero lower bound and broadcasts the stamina fraction. */
	void DrainStamina(float value);

	
	/** Enters recovery at zero stamina, regenerates until full, and broadcasts recovery transitions and the stamina fraction. */
	void UpdateStamina(float DeltaTime);
	

	
	
		
};
