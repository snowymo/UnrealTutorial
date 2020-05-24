// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceChatCharacter.h"

// Sets default values
AVoiceChatCharacter::AVoiceChatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVoiceChatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoiceChatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVoiceChatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

