// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDGameModeBase.h"

ADDGameModeBase::ADDGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//自动注册到框架
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
