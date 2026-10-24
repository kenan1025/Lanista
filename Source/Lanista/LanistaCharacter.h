// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "InputActionValue.h"
#include "Components/HealthComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/StaminaComponent.h"
#include "Items/WorldItem.h"

#include "LanistaCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
/** Carries the sprint state emitted by sprint start and stop operations outside recovery. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaStateChangedDelegate, bool, bSprinting);
/**
 *  A basic first person character
 */
UCLASS(abstract)
class ALanistaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** References the owner-only first-person mesh attached to the character's main mesh. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** References the control-rotation camera attached to the first-person mesh's head socket. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	
	

protected:

	/** Defines the input action bound to jump start and jump end. */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;
	/** Defines the input action bound to sprint start and sprint end. */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* SprintAction;


	/** Defines the two-axis input action forwarded to character movement. */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;
	/** Defines the input action that toggles crouch intent when started. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* CrouchAction;
	/** Defines the input action that starts the world-item interaction sweep. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* InteractAction;

	/** Defines the two-axis look action forwarded to controller yaw and pitch. */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Defines the mouse-look action forwarded to the same handler as LookAction. */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
public:
	
	/** Creates the first-person view and gameplay components and configures collision, camera, and movement defaults. */
	ALanistaCharacter();
	/** Applies walk speed and binds stamina recovery changes before synchronizing the current recovery speed. */
	virtual void BeginPlay() override;
	
	/** Drains sprint stamina each frame while sprint intent is active and stamina is not recovering. */
	virtual void Tick(float DeltaTime) override;
	/** Binds Enhanced Input actions for movement, aiming, jumping, sprinting, crouching, and interaction. */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
protected:

	/** Selects crouched, recovery, sprint, or normal walk speed when the stamina recovery state is synchronized. */
	void HandleRecoveryStateChanged(bool bRecovering);
	/** Cancels existing crouch intent or requests crouching when the character is not falling. */
	UFUNCTION(BlueprintCallable, Category="Input")
	void ToggleCrouch(const FInputActionValue& Value);

	/** Extracts the two-axis input value and forwards it to DoMove. */
	void MoveInput(const FInputActionValue& Value);

	/** Extracts the two-axis input value and forwards it to DoAim. */
	void LookInput(const FInputActionValue& Value);

	/** Applies yaw and pitch input when the character has a controller. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Applies movement input along the actor's right and forward vectors when a controller exists. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Requests a jump and drains JumpStaminaDrain whenever stamina is not recovering. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Ends the current jump request through StopJumping. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	/** Forwards sprint-start input to Sprint. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintStart();

	/** Forwards sprint-end input to StopSprint. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintEnd();
	
	/** Forwards interaction input to Interact. */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoInteract();
	
	/** Sweeps a camera-directed sphere and destroys a hit world item only after its full quantity is accepted by inventory. */
	void Interact();

	
	/** Defines the maximum camera-forward distance of the interaction sphere sweep. */
	UPROPERTY(EditAnywhere)
	float MaxInteractDistance = 300.0f;
	
	
	

	
	

public:
	/** References the stamina component used for sprint and jump costs and recovery speed changes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComponent;
	/** References the inventory component used to collect and drop items. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;
	/** References the health component that processes owner damage and health notifications. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;
	/** Stores the last blocking interaction hit cast to a world item, which may refer to a destroyed pickup. */
	UPROPERTY(VisibleAnywhere)
	AWorldItem* WorldItem;

	/** Defines the normal maximum walking speed applied at play start and outside sprint or recovery. */
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 300.0f;
	/** Stores sprint intent, which can remain true during stamina recovery. */
	bool bSprinting = false;
	
	
	
	/** Defines stamina drained per second while sprint intent is active outside recovery. */
	UPROPERTY(EditAnywhere, Category="Sprint")
	float SprintStaminaDrain = 10.0f;
	/** Defines stamina drained for each jump-start request accepted outside recovery. */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpStaminaDrain = 10.0f;
	/** Defines the maximum walking speed selected for sprinting outside recovery. */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, Units = "cm/s"))
	float SprintSpeed = 600.0f;
	/** Defines the maximum walking speed selected during recovery when not crouched. */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 300, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;
	/** Retains a Blueprint-writable legacy flag separate from the inherited bIsCrouched state used by C++ movement logic. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch")
	bool IsCrouched;
	
	
	
	
	/** Broadcasts sprint start or stop when the corresponding operation runs outside stamina recovery. */
	FStaminaStateChangedDelegate OnSprintStateChanged;
	
	/** Sets sprint intent unless crouched and applies sprint speed and broadcasts true only outside recovery. */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Sprint();

	/** Clears sprint intent and restores walk speed and broadcasts false only outside recovery. */
	UFUNCTION(BlueprintCallable, Category="Input")
	void StopSprint();

	
	/** Returns the owner-only first-person mesh component. */
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns the first-person camera component. */
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	

};

