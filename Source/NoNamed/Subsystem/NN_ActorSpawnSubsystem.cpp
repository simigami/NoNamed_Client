#include "NN_ActorSpawnSubsystem.h"
#include "Engine/World.h"
#include "NoNamed/Core/NN_ObjectBase.h"

void UNN_ActorSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNN_ActorSpawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

AActor* UNN_ActorSpawnSubsystem::SpawnActorFromObject(UNN_ObjectBase* SourceObject, UWorld* World, const FTransform& SpawnTransform)
{
	if (!SourceObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_ActorSpawnSubsystem::SpawnActorFromObject - SourceObject is null."));
		return nullptr;
	}

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_ActorSpawnSubsystem::SpawnActorFromObject - World context is null."));
		return nullptr;
	}

	// Delegate world spawn and assembly to the ObjectBase subclass implementation
	return SourceObject->SpawnIntoWorld(World, SpawnTransform);
}
