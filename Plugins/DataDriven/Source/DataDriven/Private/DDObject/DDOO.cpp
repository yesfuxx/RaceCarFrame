// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDOO.h"

// Add default functionality here for any IDDOO functions that are not pure virtual.
void IDDOO::RegisterToModule(FName ModName, FName ObjName, FName ClsName)
{
	//判断是否已经注册到框架了
	if (IDriver && IModule)
		return;
	//如果模组名为空，直接返回
	if (ModName.IsNone())
		return;
	//指定对象名和类名
	if (!ObjName.IsNone())
		IObjectName = ObjName;
	if (ClsName.IsNone())
		IClassName = ClsName;
	//获取UObject主体
	IBody = Cast<UObject>(this);
	//获取驱动器
	IDriver = UDDCommon::Get()->GetDriver();
	//注册到模式
	if (IDriver)
	{
		//如果获得的ID为负直接返回
		ModuleIndex = DDH::GetEnumIndexFromName(IDriver->ModuleType.ToString(), ModName);
		if (ModuleIndex < 0)
		{
			DDH::Debug() << GetObjectName() << " Get " << ModName << " ModuleIndex Failed!" << DDH::Endl();
			return;
		}
		//如果注册不成功说明还没有实例化对应的Module
		if (!IDriver->RegisterToModule(this))
			DDH::Debug() << GetObjectName() << " Register To " << ModName << " Failed !" << DDH::Endl();
	}
	else
		//DDriver不存在
		DDH::Debug() << GetObjectName() << " Get DDDriver Failed !" <<DDH::Endl();
}

FName IDDOO::GetObjectName()
{
	if (!IObjectName.IsNone())
		return IObjectName;
	IObjectName = IBody->GetFName();
	return IObjectName;
}

FName IDDOO::GetClassName()
{
	if (!IClassName.IsNone())
		return IClassName;
	IClassName = IBody->StaticClass()->GetFName();
	return IClassName;
}

int32 IDDOO::GetModuleIndex() const
{
	return ModuleIndex;
}

UObject* IDDOO::GetObjectBody() const
{
	return IBody;
}

void IDDOO::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
}

bool IDDOO::ActiveLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::None:
		DDInit();
		LifeState = EBaseObjectLife::Init;
		break;
	case EBaseObjectLife::Init:
		DDLoading();
		LifeState = EBaseObjectLife::Loading;
		break;
	case EBaseObjectLife::Loading:
		DDRegister();
		LifeState = EBaseObjectLife::Register;
		break;
	case EBaseObjectLife::Register:
		DDEnable();
		LifeState = EBaseObjectLife::Enable;
		//设置运行状态为稳定
		RunState = EBaseObjectState::Stable;
		//返回true,停止运行激活状态函数
		return true;		
	}
	return false;
}

bool IDDOO::DestroyLife()
{
}
