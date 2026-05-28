#include "NN_ActorBase.h"
#include "NN_ObjectBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

ANN_ActorBase::ANN_ActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(SceneRoot);

	VisualSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualSkeletalMesh"));
	VisualSkeletalMesh->SetupAttachment(SceneRoot);
	VisualSkeletalMesh->SetVisibility(false);
}

void ANN_ActorBase::BindSourceObject(UNN_ObjectBase* InSourceObject)
{
	SourceObject = InSourceObject;
}

void ANN_ActorBase::ApplySpawnContext(const FNN_ObjectSpawnContext& Context)
{
	ApplyIdentityContext(Context.Identity);
	ApplyMeshContext(Context.Mesh);
}

void ANN_ActorBase::ApplyIdentityContext(const FNN_ObjectIdentityContext& Identity)
{
	ObjectId = Identity.ObjectId;
	DisplayName = Identity.DisplayName;
}

void ANN_ActorBase::ApplyMeshContext(const FNN_ObjectMeshContext& Mesh) const
{
	const bool bHasStaticMesh = Mesh.StaticMesh != nullptr;
	const bool bHasSkeletalMesh = Mesh.SkeletalMesh != nullptr;

	if (VisualMesh)
	{
		VisualMesh->SetStaticMesh(Mesh.StaticMesh);
		VisualMesh->SetVisibility(bHasStaticMesh);
	}

	if (VisualSkeletalMesh)
	{
		VisualSkeletalMesh->SetSkeletalMesh(Mesh.SkeletalMesh);
		VisualSkeletalMesh->SetVisibility(bHasSkeletalMesh && !bHasStaticMesh);
	}
}
