// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldBuilderActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWorldBuilderActor::AWorldBuilderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	CollisionComp2->InitBoxExtent(FVector(5, 5, 5));
	CollisionComp2->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AMouseEventProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp2->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp2->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp2;

	// Use a ProjectileMovementComponent to govern this projectile's movement
// 	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("worldBuilder"));
// 	ProjectileMovement->UpdatedComponent = CollisionComp;

}

// Called when the game starts or when spawned
// void AWorldBuilderActor::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
// void AWorldBuilderActor::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 
// }

