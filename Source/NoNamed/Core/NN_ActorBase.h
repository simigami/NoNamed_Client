#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoNamed/Interface/NN_ObjectActorInterface.h"
#include "NN_ActorBase.generated.h"

struct FNN_ObjectMeshContext;
struct FNN_ObjectIdentityContext;
class UNN_ObjectBase;
class UStaticMeshComponent;
class USkeletalMeshComponent;

/**
 * Top-level parent for world actors (guns, bullets, etc.) created via UNN_ObjectBase spawn pipeline.
 * Implements INN_ObjectActorInterface for data-driven assembly.
 */
UCLASS()
class NONAMED_API ANN_ActorBase : public AActor, public INN_ObjectActorInterface
{
	GENERATED_BODY()

public:
	ANN_ActorBase();

	UPROPERTY(VisibleAnywhere, Category = "NN")
	TObjectPtr<UNN_ObjectBase> SourceObject;

	UPROPERTY(VisibleAnywhere, Category = "NN")
	FName ObjectId = NAME_None;

	UPROPERTY(VisibleAnywhere, Category = "NN")
	FText DisplayName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> VisualSkeletalMesh;

	//~ Begin INN_ObjectActorInterface Interface
	virtual void ApplySpawnContext(const FNN_ObjectSpawnContext& Context) override;
	virtual void BindSourceObject(UNN_ObjectBase* InSourceObject) override;
	//~ End INN_ObjectActorInterface Interface

protected:
	void ApplyIdentityContext(const FNN_ObjectIdentityContext& Identity);
	void ApplyMeshContext(const FNN_ObjectMeshContext& Mesh) const;
};
