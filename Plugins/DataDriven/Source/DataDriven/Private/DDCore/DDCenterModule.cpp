// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDCenterModule.h"
#include "DDObject/DDOO.h"

void UDDCenterModule::IterChangeModuleType(UDDModule* Module, FName ModType)
{
	Module->ChangeModuleType(ModType);
	for (USceneComponent* Child : Module->GetAttachChildren())
	{
		UDDModule* ChildModule = Cast<UDDModule>(Child);
		if (ChildModule)
		{
			//添加这个对象到Module的ChildModule
			Module->ChildrenModule.Push(ChildModule);
			IterChangeModuleType(ChildModule, ModType);
		}			
	}
}

void UDDCenterModule::IterCreateManager(UDDModule* Module)
{
	Module->CreateManager();
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
		IterCreateManager(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleInit(UDDModule* Module)
{
	Module->ModuleInit();
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
		IterModuleInit(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleBeginPlay(UDDModule* Module)
{
	Module->ModuleBeginPlay();
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
		IterModuleBeginPlay(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleTick(UDDModule* Module, float DeltaSeconds)
{
	Module->ModuleTick(DeltaSeconds);
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
		IterModuleTick(Module->ChildrenModule[i], DeltaSeconds);
}

void UDDCenterModule::TotalGatherModule(FName ModType)
{
	if (ModType.IsNone())
		return;

	//先获取到所有的模组到GatherGroup
	TArray<UDDModule*> GatherGroup;
	IterGatherModule(this, GatherGroup);

	//获取枚举的元素数量
	int32 ModuleNum = FindObject<UEnum>((UObject*)ANY_PACKAGE, *(ModType.ToString()), true)->GetMaxEnumValue();
	//填充空对象到Module Group
	for (int i = 0; i < ModuleNum; ++i)
		ModuleGroup.Push(nullptr);
	//按模组ID填充模组到ModuleGroup
	for (int i = 0; i < GatherGroup.Num(); ++i)
		ModuleGroup[GatherGroup[i]->ModuleIndex] = GatherGroup[i];
	
}

void UDDCenterModule::IterGatherModule(UDDModule* Module, TArray<UDDModule*>& GatherGroup)
{
	GatherGroup.Push(Module);
	for (UDDModule* Child : Module->ChildrenModule)
		IterGatherModule(Child, GatherGroup);
}

bool UDDCenterModule::RegisterToModule(IDDOO* ObjectInst)
{
	//判断模组ID是否有效并且注册
	if (ObjectInst->GetModuleIndex() < ModuleGroup.Num() && ModuleGroup[ObjectInst->GetModuleIndex()])
	{
		ModuleGroup[ObjectInst->GetModuleIndex()]->RegisterObject(ObjectInst);
		return true;
	}
	return false;
}
