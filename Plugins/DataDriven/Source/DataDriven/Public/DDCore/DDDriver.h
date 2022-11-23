// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDCenterModule.h"
#include "DDCommon/DDCommon.h"
#include "DDDriver.generated.h"

class IDDOO;

UCLASS()
class DATADRIVEN_API ADDDriver : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDDriver();

	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//提供给资源们进行注册
	bool RegisterToModule(IDDOO* ObjectInst);

#if WITH_EDITOR
	//属性修改方法
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// 执行反射方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

public:

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "DataDriven")
	USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "DataDriven")
	UDDCenterModule* Center;

	//模组对应的枚举名字
	UPROPERTY(EditDefaultsOnly, Category = "DataDriven")
	FName ModuleType;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//注册GamePlay框架到DataDriven
	void RegisterGamePlay();

protected:

	//是否已经运行BeginPlay函数
	bool IsBeginPlay;

};
