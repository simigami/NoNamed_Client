#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NN_ObjectSpawnSubsystem.generated.h"

class UNN_ObjectBase;
class UNN_DataAsset;

/**
 * Game-wide orchestrator for creating and managing UNN_ObjectBase logic instances.
 */
UCLASS()
class NONAMED_API UNN_ObjectSpawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Creates and returns a UNN_ObjectBase via the DataAsset's CreateInstance */
	UFUNCTION(BlueprintCallable, Category = "NN|Spawning", meta = (DisplayName = "Create Instance"))
	UNN_ObjectBase* CreateObject(UNN_DataAsset* DataAsset);
};
