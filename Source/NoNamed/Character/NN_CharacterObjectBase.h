// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Core/NN_ObjectBase.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"
#include "NN_CharacterObjectBase.generated.h"

class AController;
class AActor;
class APlayerState;
class ANN_CharacterBase;

/** Detailed spawn context attached when a character is spawned */
USTRUCT(BlueprintType)
struct FNN_CharacterSpawnContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TObjectPtr<AController> Controller = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 TeamId = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName ObjectId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bInitASC = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Animation")
	FNN_CharacterAnimationContext Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Mesh")
	FNN_CharacterMeshTransformContext MeshTransform;
};

/**
 * Base logic instance for characters.
 * Orchestrates the character actor shell spawn sequence.
 */
UCLASS(Blueprintable)
class NONAMED_API UNN_CharacterObjectBase : public UNN_ObjectBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	TSubclassOf<APawn> GetPawnClass() const;

	/**
	 * Main orchestration: spawns and assembles the character.
	 * PreInit -> SpawnDeferred -> ContextSetting -> SpawnFinish -> PostInit
	 */
	virtual ANN_CharacterBase* SpawnCharacter(UWorld* World, const FTransform& SpawnTransform, const FNN_CharacterSpawnContext& SpawnContext);

protected:
	/** Step 1: pre-spawn initialization */
	virtual void PreInit(const FNN_CharacterSpawnContext& SpawnContext);

	/** Step 2: deferred spawn (actor shell only) */
	virtual ANN_CharacterBase* SpawnDeferred(UWorld* World, const FTransform& SpawnTransform, const FNN_CharacterSpawnContext& SpawnContext);

	/** Step 3: bind source object and apply context */
	virtual void ContextSetting(ANN_CharacterBase* Character, const FNN_CharacterSpawnContext& SpawnContext);

	/** Step 4: FinishSpawning */
	virtual void SpawnFinish(ANN_CharacterBase* Character);

	/** Step 5: post-spawn (ASC init, etc.) */
	virtual void PostInit(ANN_CharacterBase* Character, const FNN_CharacterSpawnContext& SpawnContext);
};
