// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDWheeledVehicle.h"

ADDWheeledVehicle::ADDWheeledVehicle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDWheeledVehicle::DDRelease()
{
	IDDOO::DDRelease();
	GetDDWorld()->DestroyActor(this);
}

void ADDWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
	//自动注册到框架
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
