#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NN_ActorSpawnSubsystem.generated.h"

class UNN_ObjectBase;
class AActor;

/**
 * Game-wide orchestrator when UNN_ObjectBase instances are spawned into the 3D world.
 * Delegates placement and assembly to ObjectBase subclass logic (SpawnIntoWorld).
 */
UCLASS()
class NONAMED_API UNN_ActorSpawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Materializes a UNN_ObjectBase at the given world transform.
	 * Actual spawn and assembly run inside ObjectBase::SpawnIntoWorld.
	 */
	UFUNCTION(BlueprintCallable, Category = "NN|Spawning", meta = (DisplayName = "Spawn Actor From Object"))
	AActor* SpawnActorFromObject(UNN_ObjectBase* SourceObject, UWorld* World, const FTransform& SpawnTransform);
};
