// Copyright Epic Games, Inc. All Rights Reserved.

#include "NN_PlayerCameraManager.h"

ANN_PlayerCameraManager::ANN_PlayerCameraManager()
{
	DefaultFOV = 90.0f;
	ViewPitchMin = -89.0f;
	ViewPitchMax = 89.0f;
	bUseClientSideCameraUpdates = true;
}
