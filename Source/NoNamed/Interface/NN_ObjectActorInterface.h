#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NN_ObjectActorInterface.generated.h"

struct FNN_ObjectSpawnContext;
class UNN_ObjectBase;

UINTERFACE(MinimalAPI, BlueprintType)
class UNN_ObjectActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Contract for all world-representation objects (AActor, ACharacter, etc.) in the UNN_ObjectBase spawn pipeline.
 * Bridges C++ single-inheritance limits so actors and characters share the same assembly lifecycle.
 */
class NONAMED_API INN_ObjectActorInterface
{
	GENERATED_BODY()

public:
	/** Binds the source UObject logic instance */
	virtual void BindSourceObject(UNN_ObjectBase* InSourceObject) = 0;

	/** Applies spawn context (identity, mesh, etc.) to the actor */
	virtual void ApplySpawnContext(const FNN_ObjectSpawnContext& Context) = 0;
};
