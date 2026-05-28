// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NN_PlayerState.generated.h"

class UNN_CharacterObjectBase;

UCLASS()
class NONAMED_API ANN_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ANN_PlayerState();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<UNN_CharacterObjectBase> CharacterObjectBase;
};