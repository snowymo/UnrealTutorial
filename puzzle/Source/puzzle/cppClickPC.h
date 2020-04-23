// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "cppClickPC.generated.h"


/**
 * 
 */
UCLASS()
class PUZZLE_API AcppClickPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AcppClickPC();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* _mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* _box;

	virtual void SetupInputComponent() override;

	void PossesClicked();
	void PossesReleased();
	void CreateCube();

	void GetMouseInfo(bool & isPressed, float& x, float& y);
	
private:
	float LocationX;
	float LocationY;
	bool _isPressed;
	
};
