// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Core/NN_ObjectBase.h"
#include "NN_CharacterObjectBase.generated.h"

class AController;
class AActor;
class APlayerState;
class ANN_CharacterBase;

/** Runtime owner and possession references */
USTRUCT(BlueprintType)
struct FNN_CharacterOwnershipContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ownership")
	TObjectPtr<AController> Controller = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ownership")
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ownership")
	TObjectPtr<APlayerState> PlayerState = nullptr;
};

/** Runtime gameplay options for one character spawn request */
USTRUCT(BlueprintType)
struct FNN_CharacterRuntimeSpawnContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
	FNN_CharacterOwnershipContext Ownership;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
	int32 TeamId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
	bool bInitASC = true;
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
	virtual ANN_CharacterBase* SpawnCharacter(UWorld* World, const FTransform& SpawnTransform, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx = nullptr);
	virtual void AssembleCharacterSpawnContext(FNN_CharacterObjectSpawnContext& OutContext) const override;
	
protected:
	/** Step 1: pre-spawn initialization */
	virtual void PreInit(const FNN_CharacterRuntimeSpawnContext* RuntimeCtx);

	/** Step 2: deferred spawn (actor shell only) */
	virtual ANN_CharacterBase* SpawnDeferred(UWorld* World, const FTransform& SpawnTransform, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx);

	/** Step 3: bind source object and apply context */
	virtual void ContextSetting(ANN_CharacterBase* Character, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx = nullptr);

	/** Step 4: FinishSpawning */
	virtual void SpawnFinish(ANN_CharacterBase* Character);

	/** Step 5: post-spawn (ASC init, etc.) */
	virtual void PostInit(ANN_CharacterBase* Character, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx);

	/** Applies optional runtime spawn data that is not owned by the DataAsset */
	virtual void AssembleRuntimeSpawnContext(ANN_CharacterBase* Character, const FNN_CharacterRuntimeSpawnContext& RuntimeCtx);
};
