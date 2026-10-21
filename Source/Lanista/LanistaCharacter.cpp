// Copyright Epic Games, Inc. All Rights Reserved.

#include "LanistaCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Lanista.h"


ALanistaCharacter::ALanistaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f),
	                                                           FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
}

void ALanistaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (StaminaComponent)
	{
		StaminaComponent->OnRecoveryStateChanged.AddUObject(this, &ALanistaCharacter::HandleRecoveryStateChanged);
		HandleRecoveryStateChanged(StaminaComponent->bRecovering);
	}
	// This searches the Blueprint components for one named "ChestMesh"
	TArray<USkeletalMeshComponent*> Components;
	GetComponents<USkeletalMeshComponent>(Components);
 
	for (USkeletalMeshComponent* Comp : Components)
	{
		if (Comp->GetName() == TEXT("ChestMesh"))
		{
			ChestArmorComponent = Comp;
			break;
		}
	}
}

void ALanistaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bSprinting && !StaminaComponent->bRecovering)
	{
		StaminaComponent->DrainStamina(SprintStaminaDrain * DeltaTime);
	}
}

void ALanistaCharacter::HandleRecoveryStateChanged(bool bRecovering)
{
	// Check for crouch state first to avoid speed bursts while low
	if (bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = bRecovering ? RecoveringWalkSpeed : (bSprinting ? SprintSpeed : WalkSpeed);
	}
}

void ALanistaCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (!GetCharacterMovement()->IsFalling())
	{
		Crouch();
	}
}


void ALanistaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALanistaCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ALanistaCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALanistaCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALanistaCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &ALanistaCharacter::LookInput);
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &ALanistaCharacter::DoSprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &ALanistaCharacter::DoSprintEnd);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ALanistaCharacter::ToggleCrouch);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ALanistaCharacter::DoInteract);
	}
	else
	{
		UE_LOG(LogLanista, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void ALanistaCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ALanistaCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ALanistaCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ALanistaCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ALanistaCharacter::DoJumpStart()
{
	// pass Jump to the character
	if (!StaminaComponent->bRecovering)
	{
		Jump();
		StaminaComponent->DrainStamina(JumpStaminaDrain);
	}
}

void ALanistaCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ALanistaCharacter::Sprint()
{
	if(bIsCrouched)
	{
		return;
	}
	//set the sprinting flag
	bSprinting = true;
	//are we out of recoverymode?
	if (!StaminaComponent->bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		OnSprintStateChanged.Broadcast(true);
	}
}

void ALanistaCharacter::StopSprint()
{
	bSprinting = false;
	// are we out of recovery mode?
	if (!StaminaComponent->bRecovering)
	{
		// set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(false);
	}
}


void ALanistaCharacter::DoSprintStart()
{
	Sprint();
}

void ALanistaCharacter::DoSprintEnd()
{
	StopSprint();
}

void ALanistaCharacter::DoInteract()
{
	Interact();
}

void ALanistaCharacter::Interact()
{
	//GetWorld()->SweepSingleByChannel();
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractDistance);
	DrawDebugLine(GetWorld(),Start,End,FColor::Red, false, 5.0f);
	FCollisionShape InteractionSphere = FCollisionShape::MakeSphere(30.0f);
	DrawDebugSphere(GetWorld(), Start,30.0f, 20,FColor::Green,false,5.0f);
	DrawDebugSphere(GetWorld(), End,30.0f, 20,FColor::Red,false,5.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_Visibility, 
		InteractionSphere,
		QueryParams
	);
	
	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Item name is %s"),*HitActor->GetActorNameOrLabel());
		WorldItem = Cast<AWorldItem>(HitActor);
		if (WorldItem)
		{
			if (WorldItem->ActorHasTag("Chest"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor has tag"));
				EquipItem(WorldItem->ItemData);
			}else
			{
				InventoryComponent->AddItem(WorldItem->ItemData,WorldItem->Quantity);
				
			}
	
				WorldItem->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Item added"));
	
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit!"));
	}

}

void ALanistaCharacter::EquipItem(UItemDefinition* NewItem)
{
	if (!NewItem || NewItem->Slot == EEquipmentSlot::None) return;
 
	// 1. Get the specific component for this slot
	USkeletalMeshComponent* TargetComponent = nullptr;
	if (NewItem->Slot == EEquipmentSlot::Chest) TargetComponent = ChestArmorComponent;
    
	if (TargetComponent)
	{
		// 2. Load and Set the Mesh from the Data Asset
		USkeletalMesh* LoadedMesh = NewItem->EquipmentMesh.LoadSynchronous();
		TargetComponent->SetSkeletalMesh(LoadedMesh);
 
		// 3. THE MAGIC STEP: Sync animations with the body
		// This makes the chestplate move with the character's breathing/running
		TargetComponent->SetLeaderPoseComponent(GetMesh());
	}
}


