// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDCharacter.h"

// Sets default values
ADDCharacter::ADDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

