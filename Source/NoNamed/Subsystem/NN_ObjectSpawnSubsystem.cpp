#include "NN_ObjectSpawnSubsystem.h"
#include "NoNamed/Data/NN_DataAsset.h"

void UNN_ObjectSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNN_ObjectSpawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UNN_ObjectBase* UNN_ObjectSpawnSubsystem::CreateObject(UNN_DataAsset* DataAsset)
{
	if (!DataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_ObjectSpawnSubsystem::CreateObject - DataAsset is null."));
		return nullptr;
	}

	// Delegate instance creation entirely to the DataAsset's CreateInstance
	return DataAsset->CreateInstance(GetGameInstance());
}
