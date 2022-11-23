// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDLevelScriptActor.h"

ADDLevelScriptActor::ADDLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	//自动注册到框架
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
