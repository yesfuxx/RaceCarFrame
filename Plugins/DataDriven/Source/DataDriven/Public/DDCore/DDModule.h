// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DDMessage.h"
#include "DDModule.generated.h"

class IDDOO;
class UDDModel;
class UDDWealth;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DATADRIVEN_API UDDModule : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDModule();

	void CreateManager();

	//Module的Init函数
	virtual void ModuleInit();

	//Module的BeginPlay函数
	virtual void ModuleBeginPlay();

	//Module的Tick函数
	virtual void ModuleTick(float DeltaSeconds);

	void ChangeModuleType(FName ModuleType);

	//注册对象到数据模块
	void RegisterObject(IDDOO* ObjectInst);

	//模组所属对象销毁自身
	void ChildDestroy(FName ObjectName);

	//反射事件批量销毁对象
	UFUNCTION()
	void DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//反射事件批量激活对象
	UFUNCTION()
	void EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//反射事件批量失活对象
	UFUNCTION()
	void DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	
	//调用模组方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	//调用对象方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);
	
	//临时方法，测试反射事件系统
	UFUNCTION()
	void TestReflect(int32 Counter, FString InfoStr, bool& BackResult);

	UFUNCTION()
	void TestNoParam();

public:

	TArray<UDDModule*> ChildrenModule;

	UPROPERTY(VisibleAnywhere, Category = "DataDriven")
	int32 ModuleIndex;

protected:

	//执行单个对象方法
	void ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行其余对象的方法
	void ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//支持相同类的区域对象方法
	void ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行类对象的方法
	void ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行其他类对象的方法
	void ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行所有对象的方法
	void ExecuteAll(DDObjectAgreement Agreement, DDParam* Param);	

protected:

	//数据模块指针
	UDDModel* Model;
	//消息模块指针
	UDDMessage* Message;
	//资源模块指针
	UDDWealth* Wealth;
};
