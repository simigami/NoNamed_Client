#include "NN_CharacterObjectBase.h"

#include "NN_CharacterBase.h"
#include "Engine/World.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"

TSubclassOf<APawn> UNN_CharacterObjectBase::GetPawnClass() const
{
	if (!DataAsset)
	{
		return nullptr;
	}

	const UNN_CharacterDataAsset* CharacterData = Cast<UNN_CharacterDataAsset>(DataAsset);
	if (!CharacterData || !CharacterData->ActorClassToSpawn)
	{
		return nullptr;
	}

	UClass* ActorClass = CharacterData->ActorClassToSpawn.Get();
	if (!ActorClass || !ActorClass->IsChildOf(ANN_CharacterBase::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_CharacterObjectBase::GetPawnClass - ActorClassToSpawn must inherit from ANN_CharacterBase."));
		return nullptr;
	}

	return ActorClass;
}

ANN_CharacterBase* UNN_CharacterObjectBase::SpawnCharacter(
	UWorld* World,
	const FTransform& SpawnTransform,
	const FNN_CharacterRuntimeSpawnContext* RuntimeCtx)
{
	if (!World)
	{
		return nullptr;
	}

	PreInit(RuntimeCtx);

	ANN_CharacterBase* NewChar = SpawnDeferred(World, SpawnTransform, RuntimeCtx);
	if (!NewChar)
	{
		return nullptr;
	}

	ContextSetting(NewChar, RuntimeCtx);
	SpawnFinish(NewChar);
	PostInit(NewChar, RuntimeCtx);

	return NewChar;
}

void UNN_CharacterObjectBase::AssembleCharacterSpawnContext(FNN_CharacterObjectSpawnContext& OutContext) const
{
	Super::AssembleCharacterSpawnContext(OutContext);
	
	if (const UNN_CharacterDataAsset* CharacterData = Cast<UNN_CharacterDataAsset>(DataAsset))
	{
		OutContext.Animation = CharacterData->AnimationContext;
		OutContext.MeshTransform = CharacterData->MeshTransformContext;
	}
}

void UNN_CharacterObjectBase::PreInit(const FNN_CharacterRuntimeSpawnContext* RuntimeCtx)
{
	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PreInit - Preparing character spawn."));
}

ANN_CharacterBase* UNN_CharacterObjectBase::SpawnDeferred(
	UWorld* World,
	const FTransform& SpawnTransform,
	const FNN_CharacterRuntimeSpawnContext* RuntimeCtx)
{
	TSubclassOf<APawn> PawnClass = GetPawnClass();
	if (!PawnClass)
	{
		return nullptr;
	}

	UClass* CharacterClass = PawnClass.Get();
	if (!CharacterClass || !CharacterClass->IsChildOf(ANN_CharacterBase::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UNN_CharacterObjectBase::SpawnDeferred - PawnClass must inherit from ANN_CharacterBase."));
		return nullptr;
	}

	return World->SpawnActorDeferred<ANN_CharacterBase>(
		CharacterClass,
		SpawnTransform,
		RuntimeCtx ? RuntimeCtx->Ownership.Owner : nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
}

void UNN_CharacterObjectBase::ContextSetting(ANN_CharacterBase* Character, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx)
{
	if (!Character)
	{
		return;
	}

	Character->BindSourceObject(this);

	FNN_CharacterObjectSpawnContext ObjectContext;
	AssembleCharacterSpawnContext(ObjectContext);
	
	Character->ApplySpawnContext(ObjectContext);
	if (RuntimeCtx)
	{
		AssembleRuntimeSpawnContext(Character, *RuntimeCtx);
	}

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::ContextSetting - Context applied."));
}

void UNN_CharacterObjectBase::SpawnFinish(ANN_CharacterBase* Character)
{
	if (!Character)
	{
		return;
	}

	Character->FinishSpawning(Character->GetTransform());
	SpawnedActor = Character;

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::SpawnFinish - FinishSpawning completed."));
}

void UNN_CharacterObjectBase::PostInit(ANN_CharacterBase* Character, const FNN_CharacterRuntimeSpawnContext* RuntimeCtx)
{
	if (!RuntimeCtx || RuntimeCtx->bInitASC)
	{
		UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PostInit - Ability system initialization hook."));
	}

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PostInit - Character spawn completed."));
}

void UNN_CharacterObjectBase::AssembleRuntimeSpawnContext(
	ANN_CharacterBase* Character,
	const FNN_CharacterRuntimeSpawnContext& RuntimeCtx)
{
	if (!Character)
	{
		return;
	}

	if (RuntimeCtx.Ownership.Owner)
	{
		Character->SetOwner(RuntimeCtx.Ownership.Owner);
	}

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::AssembleRuntimeSpawnContext - Runtime context applied. TeamId: %d, bIsPlayer: %s"),
		RuntimeCtx.TeamId,
		RuntimeCtx.bIsPlayer ? TEXT("true") : TEXT("false"));
}
