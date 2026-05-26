// Copyright Epic Games, Inc. All Rights Reserved.

#include "NN_CharacterBase.h"
#include "NN_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"

ANN_CharacterBase::ANN_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	BaseEyeHeight = 64.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = false;
	Movement->bUseControllerDesiredRotation = false;
	Movement->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	Movement->JumpZVelocity = 420.0f;
	Movement->AirControl = 0.2f;
	Movement->MaxWalkSpeed = 600.0f;
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->MaxAcceleration = 2048.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;
	Movement->BrakingFrictionFactor = 1.0f;
	Movement->GroundFriction = 8.0f;
	Movement->GravityScale = 1.0f;
	Movement->NavAgentProps.bCanCrouch = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FirstPersonCamera->bUsePawnControlRotation = true;

	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		MeshComponent->SetOwnerNoSee(true);
		MeshComponent->bCastDynamicShadow = true;
		MeshComponent->bCastHiddenShadow = true;
	}
}

void ANN_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		const ANN_PlayerController* PlayerController = Cast<ANN_PlayerController>(GetController());
		if (!PlayerController)
		{
			return;
		}

		if (PlayerController->JumpAction)
		{
			EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this,  &ACharacter::Jump);
			EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (PlayerController->MoveAction)
		{
			EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ANN_CharacterBase::Move);
		}

		if (PlayerController->LookAction)
		{
			EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ANN_CharacterBase::Look);
		}
	}
}

void ANN_CharacterBase::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller || MovementVector.IsNearlyZero()) return;

	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ANN_CharacterBase::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller || LookAxisVector.IsNearlyZero()) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
