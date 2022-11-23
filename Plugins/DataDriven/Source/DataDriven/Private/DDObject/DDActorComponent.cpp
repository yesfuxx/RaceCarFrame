// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDActorComponent.h"

// Sets default values for this component's properties
UDDActorComponent::UDDActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// 设置允许销毁
	bAllowAnyoneToDestroyMe = true;
}


// Called when the game starts
void UDDActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// 自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);
}

void UDDActorComponent::DDRelease()
{
	IDDOO::DDRelease();
	// 从组件中删除自己，并标记为准备被GC回收
	DestroyComponent();
}


