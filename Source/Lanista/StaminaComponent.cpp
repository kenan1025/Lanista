// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LanistaCharacter.h"

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

	// ...
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStaminaComponent::DoStartSprint()
{
	//set the sprinting flag
	bSprinting =true;
	//are we out of recoverymode?
	if (!bRecovering)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
		if (MyCharacter)
		{
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			
			OnSprintStateChanged.Broadcast(true);
		}
	}
}

void UStaminaComponent::DoEndSprint()
{
	bSprinting = false;
	// are we out of recovery mode?
	if (!bRecovering)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
		if (MyCharacter)
		{
			// set the default walk speed
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;

			// call the sprint state changed delegate
			OnSprintStateChanged.Broadcast(false);
		}
		
	}
}

void UStaminaComponent::SprintFixedTick()
{
	ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter)
	{
		// are we out of recovery, still have stamina and are moving faster than our walk speed?
		if (bSprinting && !bRecovering && MyCharacter->GetVelocity().Length() > 300.0f)
		{
		

			// do we still have meter to burn?
			if (SprintMeter > 0.0f)
			{
				// update the sprint meter
				SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

				// have we run out of stamina?
				if (SprintMeter <= 0.0f)
				{
					// raise the recovering flag
					bRecovering = true;
				

					// set the recovering walk speed
					MyCharacter->GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
				}
			}
		
		} else {

			// recover stamina
			SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

			if (SprintMeter >= SprintTime)
			{
				// lower the recovering flag
				bRecovering = false;

				// set the walk or sprint speed depending on whether the sprint button is down
				MyCharacter->GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : 300.0f;

				// update the sprint state depending on whether the button is down or not
				OnSprintStateChanged.Broadcast(bSprinting);
			}

		}
	}
	

	// broadcast the sprint meter updated delegate
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}

void UStaminaComponent::CalculateStamina(AActor* TargetActor,float value)
{
	if (!TargetActor)
	{
		return;
	}
	ACharacter* MyCharacter = Cast<ACharacter>(TargetActor);
	if (MyCharacter)
	{
		UCharacterMovementComponent* MoveComp = MyCharacter->GetCharacterMovement();
		if (MoveComp)
		{
		}
	}
}

