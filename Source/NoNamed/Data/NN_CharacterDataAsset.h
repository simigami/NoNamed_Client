// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NN_DataAsset.h"
#include "NN_CharacterDataAsset.generated.h"

class UAnimInstance;
class UAnimMontage;

/** Relative transform for a spawned object's mesh component */
USTRUCT(BlueprintType)
struct FNN_ObjectMeshTransformContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FVector RelativeLocation = FVector(0.0f, 0.0f, -90.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FRotator RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
};

/** Animation-related settings for a character */
USTRUCT(BlueprintType)
struct FNN_CharacterAnimationContext
{
	GENERATED_BODY()

	/** Animation Blueprint (ABP) class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimBlueprintClass = nullptr;

	/** Default montage played on hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DefaultHitMontage = nullptr;

	/** Default montage played on death */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DefaultDeathMontage = nullptr;
};

/** DataAsset-driven context applied to a spawned character */
USTRUCT(BlueprintType)
struct FNN_CharacterObjectSpawnContext :  public FNN_ObjectSpawnContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Animation")
	FNN_CharacterAnimationContext Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Mesh")
	FNN_ObjectMeshTransformContext MeshTransform;
};

/**
 * Base DataAsset for character-specific data.
 */
UCLASS()
class NONAMED_API UNN_CharacterDataAsset : public UNN_DataAsset
{
	GENERATED_BODY()
	
public:
	/** Character animation set */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FNN_CharacterAnimationContext AnimationContext;

	/** Skeletal mesh relative transform */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	FNN_ObjectMeshTransformContext MeshTransformContext;

	virtual UNN_ObjectBase* CreateInstance(UObject* Outer) override;
};
