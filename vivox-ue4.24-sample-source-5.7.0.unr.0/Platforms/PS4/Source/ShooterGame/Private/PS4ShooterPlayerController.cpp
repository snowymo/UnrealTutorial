// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "PS4ShooterPlayerController.h"
#include "SonyInputInterface.h"

APS4ShooterPlayerController::APS4ShooterPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void APS4ShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (Player != nullptr && FSlateApplication::IsInitialized() && IsLocalController())
	{
		IInputInterface* InputInterface = FSlateApplication::Get().GetInputInterface();
		if (InputInterface)
		{
			FSonyLightColorProperty LightBar(true, FColor(255, 0, 0));
			const int32 ControllerId = CastChecked<ULocalPlayer>(Player)->GetControllerId();
			InputInterface->SetDeviceProperty(ControllerId, &LightBar);
		}
	}
}
