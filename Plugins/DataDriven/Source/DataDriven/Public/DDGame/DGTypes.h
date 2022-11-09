// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DGTypes.generated.h"

#pragma region GamePart

/**************************************************************************/
/* 开发的游戏的内容放这里                Begin               开发的游戏的内容放这里*/
/**************************************************************************/

//模组约定，如果要是用DataDriven本身的UI框架，请一定要把HUD模组放在第二位，UIFrame框架管理器始终在HUD模组下
UENUM()
enum class ERCGameModule : uint8
{
	Center = 0,		//中央模组
	HUD,			//UI模组，约定放在第二位，UI框架会直接使用序号1去寻找HUD模组，如果不使用UI框架，不需要强制在第二位
	Player,			//玩家模组
};

UENUM()
enum class ERCMenuModule : uint8
{
	Center = 0,
	HUD,
};

/**************************************************************************/
/* 开发的游戏的内容放这里                 end                开发的游戏的内容放这里*/
/**************************************************************************/

#pragma endregion 

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDGTypes : public UObject
{
	GENERATED_BODY()
};
