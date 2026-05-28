#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Character/NN_CharacterObjectBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NN_CharacterSpawnSubsystem.generated.h"

class UNN_CharacterObjectBase;
class ANN_CharacterBase;

/**
 * Character-specific world spawn orchestrator.
 * Drives character-only assembly (possession, owner, ASC init, etc.) unlike generic actors.
 */
UCLASS()
class NONAMED_API UNN_CharacterSpawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Triggers the 5-step character spawn sequence (PreInit through PostInit) on CharacterObject */
	UFUNCTION(BlueprintCallable, Category = "NN|Spawning")
	ANN_CharacterBase* SpawnCharacterFromObject(UNN_CharacterObjectBase* CharacterObject, UWorld* World, const FTransform& SpawnTransform, const FNN_CharacterRuntimeSpawnContext& RuntimeCtx);
};
