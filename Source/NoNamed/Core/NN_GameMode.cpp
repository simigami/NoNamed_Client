// Copyright Epic Games, Inc. All Rights Reserved.

#include "NN_GameMode.h"

#include "NN_GameState.h"
#include "NN_PlayerState.h"
#include "NoNamed/Character/NN_CharacterBase.h"
#include "NoNamed/Character/NN_CharacterObjectBase.h"
#include "NoNamed/Character/NN_PlayerController.h"
#include "NoNamed/Subsystem/NN_CharacterSpawnSubsystem.h"
#include "NoNamed/Subsystem/NN_ObjectSpawnSubsystem.h"
#include "NoNamed/Data/NN_CharacterDataAsset.h"
#include "NoNamed/Item/NN_InventoryComponent.h"
#include "NoNamed/Character/NN_EquipmentComponent.h"
#include "NoNamed/Weapon/NN_WeaponObjectBase.h"
#include "NoNamed/Weapon/NN_WeaponDataAsset.h"
#include "UObject/ConstructorHelpers.h"

ANN_GameMode::ANN_GameMode()
{
	// DefaultPawnClass = ANN_CharacterBase::StaticClass();
	
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ANN_PlayerController::StaticClass();
	
	static ConstructorHelpers::FClassFinder<ANN_GameState> GSClassFinder(TEXT("/Game/NN/Core/NN_BP_GS"));
	if (GSClassFinder.Succeeded() && GSClassFinder.Class)
	{
		GameStateClass = GSClassFinder.Class;
	}
	else
	{
		GameStateClass = ANN_GameState::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<ANN_PlayerState> PSClassFinder(TEXT("/Game/NN/Core/NN_BP_PS"));
	if (PSClassFinder.Succeeded() && PSClassFinder.Class)
	{
		PlayerStateClass = PSClassFinder.Class;
	}
	else
	{
		PlayerStateClass = ANN_PlayerState::StaticClass();
	}
}

APawn* ANN_GameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	ANN_PlayerState* PS = NewPlayer ? NewPlayer->GetPlayerState<ANN_PlayerState>() : nullptr;
	
	UNN_CharacterSpawnSubsystem* CharacterSpawnSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UNN_CharacterSpawnSubsystem>() : nullptr;
	if (!CharacterSpawnSubsystem)
	{
		return nullptr;
	}
	
	if (!PS || !PS->CharacterObjectBase)
	{
		return nullptr;
	}

	FNN_CharacterRuntimeSpawnContext RuntimeCtx;
	RuntimeCtx.Ownership.Controller = NewPlayer;
	RuntimeCtx.Ownership.PlayerState = PS;
	RuntimeCtx.Ownership.Owner = NewPlayer;
	RuntimeCtx.bIsPlayer = true;
	
	ANN_CharacterBase* CB = CharacterSpawnSubsystem->SpawnCharacterFromObject(PS->CharacterObjectBase, GetWorld(), SpawnTransform, RuntimeCtx);
	check(CB);

	if (ANN_GameState* GS = GetGameState<ANN_GameState>())
	{
		const TArray<TObjectPtr<UNN_WeaponDataAsset>>& TestWeapons = GS->GetTestWeaponDataAssets();
		UNN_WeaponObjectBase* LastEquippedWeapon = nullptr;

		for (const TObjectPtr<UNN_WeaponDataAsset>& WeaponData : TestWeapons)
		{
			if (WeaponData)
			{
				UNN_ObjectBase* NewInstance = WeaponData->CreateInstance(CB);
				if (UNN_WeaponObjectBase* WeaponObj = Cast<UNN_WeaponObjectBase>(NewInstance))
				{
					if (UNN_InventoryComponent* InvComp = CB->FindComponentByClass<UNN_InventoryComponent>())
					{
						if (InvComp->PickupItem(WeaponObj))
						{
							LastEquippedWeapon = WeaponObj;
						}
					}
				}
			}
		}

		if (LastEquippedWeapon)
		{
			if (UNN_EquipmentComponent* EquipComp = CB->FindComponentByClass<UNN_EquipmentComponent>())
			{
				EquipComp->EquipHand(LastEquippedWeapon);
			}
		}
	}
	
	return CB;
}

UClass* ANN_GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ANN_PlayerState* PS = InController ? InController->GetPlayerState<ANN_PlayerState>() : nullptr;
	
	if (PS && PS->CharacterObjectBase)
	{
		return PS->CharacterObjectBase->GetPawnClass();
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ANN_GameMode::PostLogin(APlayerController* NewPlayer)
{
	ANN_PlayerState* PS = NewPlayer ? NewPlayer->GetPlayerState<ANN_PlayerState>() : nullptr;
	ANN_GameState* GS = GetGameState<ANN_GameState>();
	if (PS && GS)
	{
		UNN_CharacterDataAsset* CharacterDataAsset = GS->GetCharacterDataAsset();
		UNN_ObjectSpawnSubsystem* ObjectSpawnSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UNN_ObjectSpawnSubsystem>() : nullptr;
		if (CharacterDataAsset && ObjectSpawnSubsystem)
		{
			PS->CharacterObjectBase = Cast<UNN_CharacterObjectBase>(ObjectSpawnSubsystem->CreateObject(CharacterDataAsset));
		}
	}

	Super::PostLogin(NewPlayer);
}
