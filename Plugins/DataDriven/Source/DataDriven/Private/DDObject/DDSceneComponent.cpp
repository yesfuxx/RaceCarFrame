// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDSceneComponent.h"

// Sets default values for this component's properties
UDDSceneComponent::UDDSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDDSceneComponent::DDRelease()
{
	IDDOO::DDRelease();
	// 从组件中删除自己，并标记为准备被GC回收
	DestroyComponent();
}


// Called when the game starts
void UDDSceneComponent::BeginPlay()
{
	Super::BeginPlay();
	//自动注册到框架
	RegisterToModule(ModuleName, ObjectName, ClassName);	
}


