#include "NN_CharacterSpawnSubsystem.h"
#include "NoNamed/Character/NN_CharacterObjectBase.h"

void UNN_CharacterSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNN_CharacterSpawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

ANN_CharacterBase* UNN_CharacterSpawnSubsystem::SpawnCharacterFromObject(
	UNN_CharacterObjectBase* CharacterObject, 
	UWorld* World, 
	const FTransform& SpawnTransform, 
	const FNN_CharacterSpawnContext& SpawnContext)
{
	if (!CharacterObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_CharacterSpawnSubsystem::SpawnCharacterFromObject - CharacterObject is null."));
		return nullptr;
	}

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_CharacterSpawnSubsystem::SpawnCharacterFromObject - World context is null."));
		return nullptr;
	}

	// Delegate the 5-step character spawn sequence to CharacterObject
	return CharacterObject->SpawnCharacter(World, SpawnTransform, SpawnContext);
}
