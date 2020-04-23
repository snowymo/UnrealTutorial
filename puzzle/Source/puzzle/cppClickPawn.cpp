// Fill out your copyright notice in the Description page of Project Settings.

#include "cppClickPawn.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AcppClickPawn::AcppClickPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	myComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));

	myComponent->SetupAttachment(RootComponent);
	//myComponent->SetWorldScale3D(FVector(.1, 0.2, 0.5));

}

// Called when the game starts or when spawned
void AcppClickPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcppClickPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	FVector curScale = myComponent->GetComponentScale();
// 	curScale = FVector(curScale.X, curScale.Z, curScale.Y);
// 	myComponent->SetWorldScale3D(curScale);

}

// Called to bind functionality to input
void AcppClickPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AcppClickPawn::onMouseClick);

}

void AcppClickPawn::onMouseClick()
{
	FVector curPos = myComponent->GetComponentLocation();
	curPos.Z += 10;
	myComponent->SetWorldLocation(curPos);
}

