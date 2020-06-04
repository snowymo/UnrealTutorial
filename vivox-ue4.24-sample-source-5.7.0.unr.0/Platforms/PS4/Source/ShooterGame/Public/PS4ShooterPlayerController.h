// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Player/ShooterPlayerController.h"
#include "PS4ShooterPlayerController.generated.h"

UCLASS(Config = Game)
class APS4ShooterPlayerController : public AShooterPlayerController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;
};
