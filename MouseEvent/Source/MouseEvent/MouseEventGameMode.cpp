// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MouseEventGameMode.h"
#include "MouseEventHUD.h"
#include "MouseEventCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMouseEventGameMode::AMouseEventGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Script/MouseEvent.MouseEventCharacter"));//Class'/Script/MouseEvent.MouseEventCharacter'
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMouseEventHUD::StaticClass();
}
