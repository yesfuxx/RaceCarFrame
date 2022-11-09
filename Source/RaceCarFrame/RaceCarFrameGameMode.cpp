// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaceCarFrameGameMode.h"
#include "RaceCarFramePawn.h"
#include "RaceCarFrameHud.h"

ARaceCarFrameGameMode::ARaceCarFrameGameMode()
{
	DefaultPawnClass = ARaceCarFramePawn::StaticClass();
	HUDClass = ARaceCarFrameHud::StaticClass();
}
