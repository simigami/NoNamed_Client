#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NN_DataAsset.generated.h"

class UStaticMesh;
class USkeletalMesh;
class UNN_ObjectBase;

/** Identity data (ID, display name, etc.) */
USTRUCT()
struct FNN_ObjectIdentityContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Identity")
	FName ObjectId = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Identity")
	FText DisplayName;
};

/** Visual mesh data */
USTRUCT()
struct FNN_ObjectMeshContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;
};

/** Assembly context passed to actors at spawn time */
USTRUCT()
struct FNN_ObjectSpawnContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FNN_ObjectIdentityContext Identity;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FNN_ObjectMeshContext Mesh;
};

/**
 * Base class for all project DataAssets.
 * Holds the actor class used for world spawn and default context data.
 */
UCLASS()
class NONAMED_API UNN_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Identity")
	FNN_ObjectIdentityContext Identity;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FNN_ObjectMeshContext Mesh;

	/** UObject logic instance class to create from this asset */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<UNN_ObjectBase> ObjectClass;

	/** Any AActor subclass (Actor, Character, etc.) eligible for world spawn */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> ActorClassToSpawn;

	FNN_ObjectSpawnContext BuildSpawnContext() const;

	/** Creates a runtime UObject logic instance from this data asset */
	UFUNCTION(BlueprintCallable, Category = "NN|Instance")
	virtual UNN_ObjectBase* CreateInstance(UObject* Outer);
};
