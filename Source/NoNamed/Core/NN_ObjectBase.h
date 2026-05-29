#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Data/NN_DataAsset.h"
#include "NN_ObjectBase.generated.h"

struct FNN_CharacterObjectSpawnContext;
class UNN_DataAsset;
class AActor;

/**
 * Base UObject for all object instances (weapons, bullets, etc.).
 * Owns logic and data; spawns a world-representation actor when needed.
 */
UCLASS(Blueprintable)
class NONAMED_API UNN_ObjectBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	TObjectPtr<UNN_DataAsset> DataAsset;

	UPROPERTY(VisibleInstanceOnly, Category="Data")
	TObjectPtr<AActor> SpawnedActor;

	virtual void SetDataAsset(UNN_DataAsset* InDataAsset);

	virtual bool ShouldSpawnIntoWorld() const;

	virtual void AssembleActorSpawnContext(FNN_ObjectSpawnContext& OutContext) const;
	virtual void AssembleCharacterSpawnContext(FNN_CharacterObjectSpawnContext& OutContext) const;

	/** Spawns the world-representation actor via DeferredSpawn (distinct from UE SpawnActor) */
	virtual AActor* SpawnIntoWorld(UWorld* World, const FTransform& SpawnTransform);

protected:
	virtual void OnSpawnedActorCreated(AActor* Actor, const FNN_ObjectSpawnContext& Context);
};
