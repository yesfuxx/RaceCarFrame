// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDModule.h"
#include "DDCore/DDModel.h"
#include "DDCore/DDWealth.h"
#include "DDObject/DDOO.h"

// Sets default values for this component's properties
UDDModule::UDDModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UDDModule::CreateManager()
{
	//实例化组件，这里用NewObject实例化组件时不能在括号内填this，否则编辑器运行游戏退出时会崩溃
	Model = NewObject<UDDModel>();
	Message = NewObject<UDDMessage>();
	Wealth = NewObject<UDDWealth>();
	//避免销毁
	Model->AddToRoot();
	Message->AddToRoot();
	Wealth->AddToRoot();
	//指定模组
	Model->AssignModule(this);
	Message->AssignModule(this);
	Wealth->AssignModule(this);
}

void UDDModule::ModuleInit()
{
	//调用Init函数
	Model->ModelInit();
	Message->MessageInit();
	Wealth->WealthInit();
}

void UDDModule::ModuleBeginPlay()
{
	//调用BeginPlay函数
	Model->ModelBeginPlay();
	Message->MessageBeginPlay();
	Wealth->WealthBeginPlay();
}

void UDDModule::ModuleTick(float DeltaSeconds)
{
	//调用Tick函数
	Model->ModelTick(DeltaSeconds);
	Message->MessageTick(DeltaSeconds);
	Wealth->WealthTick(DeltaSeconds);
}

void UDDModule::ChangeModuleType(FName ModuleType)
{
	ModuleIndex = DDH::GetEnumIndexFromName(ModuleType.ToString(), GetFName());

	if (ModuleIndex < 0)
		DDH::Debug() << "Generate Module Index Error --> " << GetName() << DDH::Endl();
}

void UDDModule::RegisterObject(IDDOO* ObjectInst)
{
	//注册对象到数据组件
	Model->RegisterObject(ObjectInst);
	//把自己注册到对象的模组
	ObjectInst->AssignModule(this);
}

void UDDModule::ChildDestroy(FName ObjectName)
{
	Model->DestroyObject(ObjectName);
}

void UDDModule::DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DestroyObject(Agreement, TargetNameGroup);
}

void UDDModule::EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->EnableObject(Agreement, TargetNameGroup);
}

void UDDModule::DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DisableObject(Agreement, TargetNameGroup);
}

void UDDModule::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	//调用Module的UFunction
	UFunction* ExeFunc = FindFunction(Agreement.FunctionName);
	//如果方法存在
	if(ExeFunc)
	{
		//设置调用成功
		Param->CallResult = ECallResult::Succeed;
		//调用方法
		ProcessEvent(ExeFunc, Param->ParamPtr);
	}
	else
	{
		//设置方法不存在
		Param->CallResult = ECallResult::NoFunction;
	}
}

void UDDModule::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	//区分类型执行反射方法
	switch (Agreement.AgreementType)
	{
	case EAgreementType::SelfObject:
		ExecuteSelfObject(Agreement, Param);
		break;
	case EAgreementType::OtherObject:
		ExecuteOtherObject(Agreement, Param);
		break;
	case EAgreementType::ClassOtherObject:
		ExecuteClassOtherObject(Agreement, Param);
		break;
	case EAgreementType::SelfClass:
		ExecuteSelfClass(Agreement, Param);
		break;
	case EAgreementType::OtherClass:
		ExecuteOtherClass(Agreement, Param);
		break;
	case EAgreementType::All:
		ExecuteAll(Agreement, Param);
		break;
	}
}

void UDDModule::TestReflect(int32 Counter, FString InfoStr, bool& BackResult)
{
	DDH::Debug() << Counter << " ; " << InfoStr << " : " << GetFName() << DDH::Endl();

	BackResult = false;
}

void UDDModule::TestNoParam()
{
	DDH::Debug() << "No Param" << DDH::Endl();
}

void UDDModule::ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	Model->GetSelfObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//如果获取的对象有缺失，设置结果为对象缺失，这个结果的优先级最高
	if (TargetObjectGroup.Num() != Agreement.ObjectGroup.Num())
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	int32 TotalObjectNum = Model->GetOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//如果获取的对象有缺失，设置结果为对象缺失，这个结果的优先级最高
	if (TargetObjectGroup.Num() + Agreement.ObjectGroup.Num() != TotalObjectNum)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	int32 TotalClassNum = Model->GetClassOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if (TargetObjectGroup.Num() + Agreement.ObjectGroup.Num() != TotalClassNum)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	Model->GetSelfClass(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if (TargetObjectGroup.Num() == 0)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	Model->GetOtherClass(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if (TargetObjectGroup.Num() == 0)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteAll(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象得组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	Model->GetAll(TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
}


