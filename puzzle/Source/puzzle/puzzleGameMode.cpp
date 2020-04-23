// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "puzzleGameMode.h"
#include "puzzlePlayerController.h"
#include "puzzlePawn.h"

ApuzzleGameMode::ApuzzleGameMode()
{
	// no pawn by default
	DefaultPawnClass = ApuzzlePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ApuzzlePlayerController::StaticClass();
}
