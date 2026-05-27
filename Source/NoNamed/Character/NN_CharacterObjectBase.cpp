// Fill out your copyright notice in the Description page of Project Settings.


#include "NN_CharacterObjectBase.h"

#include "NN_CharacterBase.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"

TSubclassOf<APawn> UNN_CharacterObjectBase::GetPawnClass() const
{
	if (!DataAsset) return nullptr;
	
	if (UNN_CharacterDataAsset* CDA = Cast<UNN_CharacterDataAsset>(DataAsset))
	{
		if (CDA->ActorClassToSpawn && CDA->ActorClassToSpawn->IsChildOf(ANN_CharacterBase::StaticClass()))
		{
			return TSubclassOf<APawn>(CDA->ActorClassToSpawn);
		}
	}
	
	return nullptr;
}

ANN_CharacterBase* UNN_CharacterObjectBase::SpawnCharacter(
	UWorld* World, 
	const FTransform& SpawnTransform, 
	const FNN_CharacterSpawnContext& SpawnContext)
{
	if (!World)
	{
		return nullptr;
	}

	// 1단계: PreInit
	PreInit(SpawnContext);

	// 2단계: SpawnDeferred (지연 스폰으로 껍데기 소환)
	ANN_CharacterBase* SpawnedChar = SpawnDeferred(World, SpawnTransform, SpawnContext);
	if (!SpawnedChar)
	{
		return nullptr;
	}

	// 3단계: Context Setting (컨텍스트 세팅 및 조립)
	ContextSetting(SpawnedChar, SpawnContext);

	// 4단계: Spawn Finish (실제 스폰 완료 처리)
	SpawnFinish(SpawnedChar);

	// 5단계: PostInit (스폰 완료 후 어빌리티 초기화 등 후처리)
	PostInit(SpawnedChar, SpawnContext);

	return SpawnedChar;
}

void UNN_CharacterObjectBase::PreInit(const FNN_CharacterSpawnContext& SpawnContext)
{
	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PreInit - Preparing spawn context."));
}

ANN_CharacterBase* UNN_CharacterObjectBase::SpawnDeferred(
	UWorld* World, 
	const FTransform& SpawnTransform, 
	const FNN_CharacterSpawnContext& SpawnContext)
{
	TSubclassOf<APawn> PawnClass = GetPawnClass();
	if (!PawnClass)
	{
		return nullptr;
	}

	TSubclassOf<ANN_CharacterBase> CharacterClass = TSubclassOf<ANN_CharacterBase>(PawnClass);
	if (!CharacterClass)
	{
		return nullptr;
	}

	// DeferredSpawn 호출
	ANN_CharacterBase* NewCharacter = World->SpawnActorDeferred<ANN_CharacterBase>(
		CharacterClass,
		SpawnTransform,
		SpawnContext.Owner,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	return NewCharacter;
}

void UNN_CharacterObjectBase::ContextSetting(ANN_CharacterBase* Character, const FNN_CharacterSpawnContext& SpawnContext)
{
	if (!Character)
	{
		return;
	}
	
	// 1. 소스 오브젝트 바인딩 및 기본 에셋 조립 (기존 인터페이스 메커니즘 활용)
	Character->BindSourceObject(this);
	Character->ApplySpawnContext(AssembleSpawnContext());

	// 2. FNN_CharacterSpawnContext 정보를 캐릭터에 적용
	Character->ObjectId = SpawnContext.ObjectId;
	Character->DisplayName = SpawnContext.DisplayName;

	// 3. 애니메이션 컨텍스트 바인딩 (Context 우선, 없으면 DataAsset 폴백)
	FNN_CharacterAnimationContext AnimContext = SpawnContext.Animation;
	if (!AnimContext.AnimBlueprintClass)
	{
		if (const UNN_CharacterDataAsset* CharacterDataAsset = Cast<UNN_CharacterDataAsset>(DataAsset))
		{
			AnimContext = CharacterDataAsset->AnimationContext;
		}
	}
	Character->ApplyCharacterAnimation(AnimContext);

	// 4. 메쉬 Relative Transform 바인딩 (DataAsset 기본, SpawnContext로 런타임 오버라이드 가능)
	FNN_CharacterMeshTransformContext MeshTransformContext;
	if (const UNN_CharacterDataAsset* CharacterDataAsset = Cast<UNN_CharacterDataAsset>(DataAsset))
	{
		MeshTransformContext = CharacterDataAsset->MeshTransformContext;
	}

	const bool bHasSpawnMeshTransformOverride =
		!SpawnContext.MeshTransform.RelativeLocation.IsNearlyZero() ||
		!SpawnContext.MeshTransform.RelativeRotation.IsNearlyZero();

	if (bHasSpawnMeshTransformOverride)
	{
		MeshTransformContext = SpawnContext.MeshTransform;
	}

	Character->ApplyCharacterMeshTransform(MeshTransformContext);

	// 주입받은 컨트롤러 설정 (필요시 빙의 사전 작업 등)
	if (SpawnContext.Controller)
	{
		// 예를 들어 Controller 캐싱 처리 등 가능
	}

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::ContextSetting - Spawn Context applied (TeamId: %d, bIsPlayer: %s)"), 
		SpawnContext.TeamId, SpawnContext.bIsPlayer ? TEXT("true") : TEXT("false"));
}

void UNN_CharacterObjectBase::SpawnFinish(ANN_CharacterBase* Character)
{
	if (!Character)
	{
		return;
	}

	// 물리 및 비주얼 활성화와 함께 스폰 완료 처리
	Character->FinishSpawning(Character->GetTransform());
	
	// 소유 변수에 스폰된 액터 보관
	SpawnedActor = Character;

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::SpawnFinish - FinishSpawning executed successfully."));
}

void UNN_CharacterObjectBase::PostInit(ANN_CharacterBase* Character, const FNN_CharacterSpawnContext& SpawnContext)
{
	// ASC(어빌리티 시스템 컴포넌트) 초기화 플래그 처리 및 컴포넌트 추가 초기화
	if (SpawnContext.bInitASC)
	{
		// 예: Character->InitializeAbilitySystemComponent() 호출 등 수행 가능
		UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PostInit - Ability System Component initialized."));
	}

	UE_LOG(LogTemp, Log, TEXT("UNN_CharacterObjectBase::PostInit - Character spawn sequence fully completed."));
}
