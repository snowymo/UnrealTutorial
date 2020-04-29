// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "ProcMeshActor.h"
#include "FPFloorPawn.generated.h"

UCLASS()
class FPPROCMESH_API AFPFloorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFPFloorPawn();

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* BoardMesh;

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, Category = Mesh)
	TSubclassOf<class AProcMeshActor> dynamicMeshClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to handle mouse press
	void HandleLeftMousePress();
	void HandleLeftMouseRelease();

private:
	APlayerController* _playerCtrl;
	FVector _meshStart, _meshEnd;
	int _mouseState;
	AProcMeshActor* lastMesh;
	
	
};
