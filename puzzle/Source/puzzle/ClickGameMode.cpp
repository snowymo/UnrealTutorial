// Fill out your copyright notice in the Description page of Project Settings.

#include "ClickGameMode.h"
#include "cppClickPC.h"



AClickGameMode::AClickGameMode()
{
	// no pawn by default
	//DefaultPawnClass = ApuzzlePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AcppClickPC::StaticClass();
}
