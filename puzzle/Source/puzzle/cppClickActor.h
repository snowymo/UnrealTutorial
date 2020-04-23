// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cppClickActor.generated.h"

UCLASS()
class PUZZLE_API AcppClickActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcppClickActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//SetupPlayerInputComponent(class UInputComponent* InputComponent);

private:
	// 
	void onMousePressed();
	bool _isPressed;
	float _x, _y;
	
	
};
