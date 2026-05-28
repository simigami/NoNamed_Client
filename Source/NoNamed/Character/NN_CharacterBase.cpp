// Copyright Epic Games, Inc. All Rights Reserved.

#include "NN_CharacterBase.h"
#include "NN_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Nonamed/Core/NN_ObjectBase.h"
#include "Nonamed/Data/NN_CharacterDataAsset.h"

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

void ANN_CharacterBase::BindSourceObject(UNN_ObjectBase* InSourceObject)
{
	SourceObject = InSourceObject;
}

void ANN_CharacterBase::ApplySpawnContext(const FNN_ObjectSpawnContext& Context)
{
	ApplyIdentityContext(Context.Identity);
	ApplyMeshContext(Context.Mesh);
}

void ANN_CharacterBase::ApplySpawnContext(const FNN_CharacterObjectSpawnContext& Context)
{
	ApplySpawnContext(static_cast<const FNN_ObjectSpawnContext&>(Context));
	ApplyAnimationContext(Context.Animation);
	ApplyCharacterMeshTransform(Context.MeshTransform);
}

void ANN_CharacterBase::ApplyIdentityContext(const FNN_ObjectIdentityContext& Identity)
{
	ObjectId = Identity.ObjectId;
	DisplayName = Identity.DisplayName;
}

void ANN_CharacterBase::ApplyMeshContext(const FNN_ObjectMeshContext& OutMesh) const
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		if (OutMesh.SkeletalMesh)
		{
			CharacterMesh->SetSkeletalMesh(OutMesh.SkeletalMesh);
			CharacterMesh->SetVisibility(false);
		}
	}
}

void ANN_CharacterBase::ApplyCharacterMeshTransform(const FNN_ObjectMeshTransformContext& MeshTransformContext)
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetRelativeLocation(MeshTransformContext.RelativeLocation);
		CharacterMesh->SetRelativeRotation(MeshTransformContext.RelativeRotation);

		if (CharacterMesh->GetSkeletalMeshAsset())
		{
			CharacterMesh->SetVisibility(true);
		}
	}
}

void ANN_CharacterBase::ApplyCharacterAnimation(const FNN_CharacterAnimationContext& AnimContext)
{
	AnimBlueprintClass = AnimContext.AnimBlueprintClass;
	DefaultHitMontage = AnimContext.DefaultHitMontage;
	DefaultDeathMontage = AnimContext.DefaultDeathMontage;

	// Bind AnimBlueprint class to the character skeletal mesh
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		if (AnimBlueprintClass)
		{
			CharacterMesh->SetAnimInstanceClass(AnimBlueprintClass);
			UE_LOG(LogTemp, Log, TEXT("ANN_CharacterBase::ApplyCharacterAnimation - Applied AnimInstance Class successfully."));
		}
	}
}

void ANN_CharacterBase::ApplyAnimationContext(const FNN_CharacterAnimationContext& AnimContext)
{
	ApplyCharacterAnimation(AnimContext);
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
