// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDCore/DDModule.h"
#include "DDCenterModule.generated.h"

class IDDOO;

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDCenterModule : public UDDModule
{
	GENERATED_BODY()

public:

	//迭代调用本模组以及模组的EditchangeModuleType方法
	void IterChangeModuleType(UDDModule* Module, FName ModType);
	//递归创建模块
	void IterCreateManager(UDDModule* Module);
	//递归初始化
	void IterModuleInit(UDDModule* Module);
	//递归BeginPlay
	void IterModuleBeginPlay(UDDModule* Module);
	//递归Tick
	void IterModuleTick(UDDModule* Module, float DeltaSeconds);
	//提取所有模组到模组数组，传入枚举名字
	void TotalGatherModule(FName ModType);
	//提取所有模组到数组
	void IterGatherModule(UDDModule* Module, TArray<UDDModule*>& GatherGroup);
	//注册对象到模组
	bool RegisterToModule(IDDOO* ObjectInst);
	
protected:

	//保存模组的数组，顺序与枚举相同
	TArray<UDDModule*> ModuleGroup;
	
};
