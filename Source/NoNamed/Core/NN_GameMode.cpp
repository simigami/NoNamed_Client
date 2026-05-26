// Copyright Epic Games, Inc. All Rights Reserved.

#include "NN_GameMode.h"
#include "NN_GameState.h"
#include "NN_PlayerState.h"
#include "NoNamed/Character/NN_CharacterBase.h"
#include "NoNamed/Character/NN_PlayerController.h"

ANN_GameMode::ANN_GameMode()
{
	DefaultPawnClass = ANN_CharacterBase::StaticClass();
	PlayerControllerClass = ANN_PlayerController::StaticClass();
	GameStateClass = ANN_GameState::StaticClass();
	PlayerStateClass = ANN_PlayerState::StaticClass();
}
