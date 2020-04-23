// Fill out your copyright notice in the Description page of Project Settings.

#include "cppClickPC.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include <Engine/Engine.h>
#include "Components/BoxComponent.h"
#include "UObject/UObjectGlobals.h"


AcppClickPC::AcppClickPC()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = _mesh;

	_isPressed = false;

	CreateCube();
	
}

void AcppClickPC::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AcppClickPC::PossesClicked);
	InputComponent->BindAction("Fire", IE_Released, this, &AcppClickPC::PossesReleased);
}

void AcppClickPC::PossesClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == NULL) {
		GEngine->AddOnScreenDebugMessage(2, 0, FColor::Red, TEXT("no PC"));
	}
	else {
		
		// Get the coordinates of the mouse from our controller  
// 		float prevX = LocationX, prevY = LocationY;
 		PlayerController->GetMousePosition(LocationX, LocationY);
// 		FVector curScale = _mesh->GetComponentScale();
// 		curScale.X *= LocationX / prevX;
// 		curScale.Y *= LocationY / prevY;
// 		_mesh->SetRelativeScale3D(curScale);
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("clicked:" + FString::FromInt(LocationX) + "," + FString::FromInt(LocationY)));
		//CreateCube();
	}	
	_isPressed = true;
}

void AcppClickPC::PossesReleased()
{
	_isPressed = false;
}

void AcppClickPC::CreateCube()
{
	_box = CreateDefaultSubobject<UBoxComponent>(TEXT("cube"));
	//(X=-295.000000,Y=195.000000,Z=225.000000)
	
	_box->SetupAttachment(RootComponent);
	_box->SetWorldLocation(FVector( -295.000000,  195.000000, 225.000000));
	_box->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));
}

void AcppClickPC::GetMouseInfo(bool & isPressed, float& x, float& y)
{
	isPressed = _isPressed;
	x = LocationX;
	y = LocationY;
}
