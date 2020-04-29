// Fill out your copyright notice in the Description page of Project Settings.

#include "FPFloorPawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


// Sets default values
AFPFloorPawn::AFPFloorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPFloorPawn::BeginPlay()
{
	Super::BeginPlay();

	_playerCtrl = Cast<APlayerController>(GetController());
	_mouseState = 0;
	
}

// Called every frame
void AFPFloorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_playerCtrl && _mouseState >= 1) {

 		FHitResult TraceResult(ForceInit);
 		_playerCtrl->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);
 		FString TraceString("_mouseState >= 1");
		if (TraceResult.GetActor() != nullptr)
		{
			if (_mouseState == 1) {
				_meshStart = TraceResult.Location;
				_mouseState = 2;
			}
			_meshEnd = TraceResult.Location;
			lastMesh->UpdateMeshWithDiagonal(_meshStart, _meshEnd);
			TraceString += FString::Printf(TEXT("Trace Actor %s at (%3.2f,%3.2f,%3.2f)"), *TraceResult.GetActor()->GetName(), _meshEnd.X, _meshEnd.Y, _meshEnd.Z);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TraceString);
	}
	else {
		if (!_playerCtrl) {
			_playerCtrl = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());//Cast<AFPProcMeshCharacter>(pWorld->GetFirstPlayerController());
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(" NO _playerCtrl"));
		}
 			
//  		if (_mouseState == 0)
//  			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(" NO _mouseState"));
	}

}

// Called to bind functionality to input
void AFPFloorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPFloorPawn::HandleLeftMousePress);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPFloorPawn::HandleLeftMouseRelease);
}

void AFPFloorPawn::HandleLeftMousePress()
{
	_mouseState = 1;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("in handle left mouse pressed"));

	// try and fire a projectile
	if (dynamicMeshClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			AProcMeshActor* newMesh = World->SpawnActor<AProcMeshActor>(dynamicMeshClass, FVector(0,0,5), FRotator::ZeroRotator);
			
			//if(newMesh != NULL)
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SpawnActor"));
  			newMesh->CreateMeshWithDiagonal(_meshStart, _meshEnd);
			//newMesh->GeneratePoints();
  			newMesh->GenerateCubeMesh();
			lastMesh = newMesh;
		}
	}
}

void AFPFloorPawn::HandleLeftMouseRelease()
{
	_mouseState = 0;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("in handle left mouse released"));
}

