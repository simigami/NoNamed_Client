// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"
#include "NoNamed/Data/NN_DataAsset.h"
#include "NoNamed/Interface/NN_ObjectActorInterface.h"
#include "NN_CharacterBase.generated.h"

struct FInputActionValue;
struct FNN_CharacterObjectSpawnContext;
class UCameraComponent;
class UNN_ObjectBase;
class UAnimInstance;
class UAnimMontage;

/**
 * Base class for all characters (player, AI, etc.).
 * Implements INN_ObjectActorInterface to assemble from UNN_ObjectBase data and logic.
 */
UCLASS()
class NONAMED_API ANN_CharacterBase : public ACharacter, public INN_ObjectActorInterface
{
	GENERATED_BODY()

public:
	ANN_CharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	/** Source UObject logic instance that owns this character */
	UPROPERTY(VisibleAnywhere, Category = "NN")
	TObjectPtr<UNN_ObjectBase> SourceObject;

	UPROPERTY(VisibleAnywhere, Category = "NN")
	FName ObjectId = NAME_None;

	UPROPERTY(VisibleAnywhere, Category = "NN")
	FText DisplayName;

	//~ Begin Animation Info Cached from DataAsset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimBlueprintClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DefaultHitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DefaultDeathMontage;
	//~ End Animation Info

	//~ Begin INN_ObjectActorInterface Interface
	virtual void ApplySpawnContext(const FNN_ObjectSpawnContext& Context) override;
	virtual void ApplySpawnContext(const FNN_CharacterObjectSpawnContext& Context);
	virtual void BindSourceObject(UNN_ObjectBase* InSourceObject) override;
	//~ End INN_ObjectActorInterface Interface

	/** Applies character animation data */
	virtual void ApplyCharacterAnimation(const FNN_CharacterAnimationContext& AnimContext);
	virtual void ApplyAnimationContext(const FNN_CharacterAnimationContext& AnimContext);

	/** Applies skeletal mesh relative transform; shows mesh after transform is set */
	virtual void ApplyCharacterMeshTransform(const FNN_ObjectMeshTransformContext& MeshTransformContext);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ApplyIdentityContext(const FNN_ObjectIdentityContext& Identity);
	void ApplyMeshContext(const FNN_ObjectMeshContext& Mesh) const;
};
