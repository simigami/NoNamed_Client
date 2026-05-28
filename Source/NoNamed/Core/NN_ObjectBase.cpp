#include "NN_ObjectBase.h"
#include "Engine/World.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"
#include "NoNamed/Data/NN_DataAsset.h"
#include "NoNamed/Interface/NN_ObjectActorInterface.h"

void UNN_ObjectBase::SetDataAsset(UNN_DataAsset* InDataAsset)
{
	DataAsset = InDataAsset;
}

bool UNN_ObjectBase::ShouldSpawnIntoWorld() const
{
	return DataAsset != nullptr && DataAsset->ActorClassToSpawn != nullptr;
}

void UNN_ObjectBase::AssembleActorSpawnContext(FNN_ObjectSpawnContext& OutContext) const
{
	if (DataAsset)
	{
		DataAsset->BuildSpawnContext(OutContext);
	}
}

void UNN_ObjectBase::AssembleCharacterSpawnContext(FNN_CharacterObjectSpawnContext& OutContext) const
{
	if (DataAsset)
	{
		DataAsset->BuildSpawnContext(OutContext);
	}
}

void UNN_ObjectBase::OnSpawnedActorCreated(AActor* Actor, const FNN_ObjectSpawnContext& Context)
{
	if (!Actor)
	{
		return;
	}

	// Bind and apply context if the actor implements the interface
	if (INN_ObjectActorInterface* ObjectActor = Cast<INN_ObjectActorInterface>(Actor))
	{
		ObjectActor->BindSourceObject(this);
		ObjectActor->ApplySpawnContext(Context);
	}
}

AActor* UNN_ObjectBase::SpawnIntoWorld(UWorld* World, const FTransform& SpawnTransform)
{
	if (!World || !ShouldSpawnIntoWorld())
	{
		return nullptr;
	}

	FNN_ObjectSpawnContext Context;
	AssembleActorSpawnContext(Context);
	TSubclassOf<AActor> ActorClass = DataAsset->ActorClassToSpawn;
	if (!ActorClass)
	{
		return nullptr;
	}

	AActor* NewActor = World->SpawnActorDeferred<AActor>(
		ActorClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!NewActor)
	{
		return nullptr;
	}

	OnSpawnedActorCreated(NewActor, Context);
	NewActor->FinishSpawning(SpawnTransform);

	SpawnedActor = NewActor;
	return NewActor;
}
