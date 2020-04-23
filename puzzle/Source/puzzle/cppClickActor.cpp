// Fill out your copyright notice in the Description page of Project Settings.

#include "cppClickPC.h"
#include "cppClickActor.h"
#include "Engine/World.h"

#include "GameFramework/PlayerController.h"

// Sets default values
AcppClickActor::AcppClickActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//InputComponent->BindAction("Fire", IE_Pressed, this, &AcppClickActor::onMousePressed);

}

// Called when the game starts or when spawned
void AcppClickActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcppClickActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AcppClickPC* PlayerController = Cast<AcppClickPC*> (GetWorld()->GetFirstPlayerController());
	if (PlayerController != NULL) {
		//
		PlayerController->GetMouseInfo(_isPressed, _x, _y);
		if (_isPressed) {
			GEngine->AddOnScreenDebugMessage(2, 2, FColor::Blue, TEXT("clicked:" + FString::FromInt(_x) + "," + FString::FromInt(_y)));
		}
	}
}

void AcppClickActor::onMousePressed()
{

}

