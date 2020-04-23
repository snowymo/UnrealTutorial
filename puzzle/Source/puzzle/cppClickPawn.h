// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "cppClickPawn.generated.h"

UCLASS()
class PUZZLE_API AcppClickPawn : public APawn
{
	GENERATED_BODY()

		/** Dummy root component */
// 		UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
// 		class USceneComponent* DummyRoot;

public:
	// Sets default values for this pawn's properties
	AcppClickPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class USceneComponent* myComponent;

	void onMouseClick();


	
};
