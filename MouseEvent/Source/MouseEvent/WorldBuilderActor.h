// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MouseEventProjectile.h"
#include "WorldBuilderActor.generated.h"

UCLASS()
class MOUSEEVENT_API AWorldBuilderActor : public AActor
{
	GENERATED_BODY()

		/** Projectile movement component */
		//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		//class UProjectileMovementComponent* ProjectileMovement;

	/** box collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class UBoxComponent* CollisionComp2;
	
public:	
	// Sets default values for this actor's properties
	AWorldBuilderActor();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/** Returns ProjectileMovement subobject **/
	//FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	
	
};
