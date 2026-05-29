// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NN_GameState.generated.h"

class UNN_WeaponDataAsset;
class UNN_CharacterDataAsset;

UCLASS()
class NONAMED_API ANN_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UNN_CharacterDataAsset* GetCharacterDataAsset() const;

	const TArray<TObjectPtr<UNN_WeaponDataAsset>>& GetTestWeaponDataAssets() const { return TestWeaponDataAssets; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TObjectPtr<UNN_CharacterDataAsset> CharacterDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TObjectPtr<UNN_WeaponDataAsset>> TestWeaponDataAssets;
};
