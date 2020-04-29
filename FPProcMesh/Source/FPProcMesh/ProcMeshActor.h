// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FPProcMesh/FPProcMeshCharacter.h"
#include "Engine/World.h"
#include "ProcMeshActor.generated.h"

UCLASS()
class FPPROCMESH_API AProcMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcMeshActor();

	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	class AFPProcMeshCharacter* theCharacter;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3);

	void GeneratePoints();
	void GenerateCubeMesh();

	void UpdateCubeMesh(const FRotator& rot, const FVector& vec);

	void CreateMeshWithDiagonal(FVector startPt, FVector endPt);
	void UpdateMeshWithDiagonal(FVector startPt, FVector endPt);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
