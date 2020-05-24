// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcMeshActor.h"
#include <algorithm>


// Sets default values
AProcMeshActor::AProcMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;

	auto pWorld = GetWorld();
	if (pWorld)
	{
		theCharacter = Cast<AFPProcMeshCharacter>(pWorld->GetFirstPlayerController());
	}

}

void AProcMeshActor::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);	
}

void AProcMeshActor::GeneratePoints()
{
	//These are relative locations to the placed Actor in the world
	Vertices.Add(FVector(0, -100, 0)); //lower left - 0
	Vertices.Add(FVector(0, -100, 100)); //upper left - 1
	Vertices.Add(FVector(0, 100, 0)); //lower right - 2 
	Vertices.Add(FVector(0, 100, 100)); //upper right - 3

	Vertices.Add(FVector(100, -100, 0)); //lower front left - 4
	Vertices.Add(FVector(100, -100, 100)); //upper front left - 5

	Vertices.Add(FVector(100, 100, 100)); //upper front right - 6
	Vertices.Add(FVector(100, 100, 0)); //lower front right - 7
}

void AProcMeshActor::GenerateCubeMesh()
{
	// locate where to place cube
	//GeneratePoints();	

	//6 sides on cube, 4 verts each (corners)	

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

void AProcMeshActor::UpdateCubeMesh(const FRotator& rot, const FVector& vec)
{
	FVector deltaPos = vec - Vertices[0];
	//Moving the mesh
	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		//Moving each vertex 1 unit forward
		Vertices[index] += deltaPos;
	}
	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void AProcMeshActor::CreateMeshWithDiagonal(FVector startPt, FVector endPt)
{
	FVector delta = endPt - startPt;
	delta.Z = std::max(10.0f, delta.Z);
	delta.X = std::max(1.0f, delta.X);
	delta.Y = std::max(1.0f, delta.Y);

	//These are relative locations to the placed Actor in the world
	Vertices.Add(startPt + FVector(delta.X, 0, 0)); //lower left - 0
	Vertices.Add(startPt + FVector(delta.X, 0, delta.Z));//upper left - 1
	Vertices.Add(startPt + FVector(delta.X, delta.Y, 0)); //lower right - 2 
	Vertices.Add(startPt + FVector(delta.X, delta.Y, delta.Z)); //upper right - 3

	Vertices.Add(startPt);  //lower front left - 4
	Vertices.Add(startPt + FVector(0, 0, delta.Z)); //upper front left - 5

	Vertices.Add(startPt + FVector(0, delta.Y, delta.Z)); //upper front right - 6
	Vertices.Add(startPt + FVector(0, delta.Y, 0)); //lower front right - 7
}

void AProcMeshActor::UpdateMeshWithDiagonal(FVector startPt, FVector endPt)
{
	// endpoint is changing
	FVector delta = endPt - startPt;
	delta.Z = std::max(10.0f, delta.Z);
	delta.X = std::max(1.0f, delta.X);
	delta.Y = std::max(1.0f, delta.Y);

	//These are relative locations to the placed Actor in the world
	Vertices[0] = (startPt + FVector(delta.X, 0, 0)); //lower left - 0
	Vertices[1] = (startPt + FVector(delta.X, 0, delta.Z));//upper left - 1
	Vertices[2] = (startPt + FVector(delta.X, delta.Y, 0)); //lower right - 2 
	Vertices[3] = (startPt + FVector(delta.X, delta.Y, delta.Z)); //upper right - 3

	Vertices[4] = (startPt);  //lower front left - 4
	Vertices[5] = (startPt + FVector(0, 0, delta.Z)); //upper front left - 5
	Vertices[6] =(startPt + FVector(0, delta.Y, delta.Z)); //upper front right - 6
	Vertices[7] = (startPt + FVector(0, delta.Y, 0)); //lower front right - 7

}

// Called when the game starts or when spawned
void AProcMeshActor::BeginPlay()
{
	Super::BeginPlay();

// 	GeneratePoints();
// 	GenerateCubeMesh();
	
}


// Called every frame
void AProcMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

