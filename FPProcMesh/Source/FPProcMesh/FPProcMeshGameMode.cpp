// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPProcMeshGameMode.h"
#include "FPProcMeshHUD.h"
#include "FPProcMeshCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPProcMeshGameMode::AFPProcMeshGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPProcMeshHUD::StaticClass();
}
