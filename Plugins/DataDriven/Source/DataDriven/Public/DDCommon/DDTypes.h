// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/GameEngine.h"
#include "DDTypes.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDTypes : public UObject
{
	GENERATED_BODY()
};


//框架自定义Debug输出类
#pragma region LogDebug

class DATADRIVEN_API DDRecord
{
private:
	//自身单例
	static TSharedPtr<DDRecord> RecordInst;

	//生成的字符串
	FString RecordInfo;

	//显示时间
	float ShowTime;
	
	//显示颜色
	FColor ShowColor;

public:

	//打印模式，0：Debug，1：Log，2：Warning 3：Error
	uint8 PatternID;

public:

	//构造函数
	inline DDRecord(){}

	~DDRecord(){};

	static TSharedPtr<DDRecord> Get();

	//设定参数
	inline void InitParam(float InTime, FColor InColor)
	{
		ShowTime = InTime;
		ShowColor = InColor;
	}

	//执行打印方法
	inline void Output()
	{
		switch (PatternID)
		{
		case 0:
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, ShowTime, ShowColor, RecordInfo);
			}
			break;
		case 1:
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *RecordInfo);
			}
			break;
		case 2:
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *RecordInfo);
			}
			break;
		case 3:
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *RecordInfo);
			}
			break;
		}
		//清空字符串
		RecordInfo.Empty();
	}

	//重写操作符<<
	inline DDRecord &operator<<(FString Info){ RecordInfo.Append(Info); return *this; }
	inline DDRecord &operator<<(FName Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FText Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(const char* Info){ RecordInfo += Info; return *this; }
	inline DDRecord &operator<<(const char Info){ RecordInfo.AppendChar(Info); return *this; }
	inline DDRecord &operator<<(int32 Info){ RecordInfo.Append(FString::FromInt(Info)); return *this; }
	inline DDRecord &operator<<(float Info){ RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord &operator<<(double Info){ RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord &operator<<(bool Info){ RecordInfo.Append(Info ? FString("true") : FString("false")); return *this; }
	inline DDRecord &operator<<(FVector2D Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FVector Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FRotator Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FQuat Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FTransform Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FMatrix Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FColor Info){ RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FLinearColor Info){ RecordInfo.Append(Info.ToString()); return *this; }

	//输出
	inline void operator<<(DDRecord& Record){ Record.Output(); }
};

// DDRecord::Get() << FString("sssss") << FName << int << float << DDRecord::Get();

#pragma endregion

#pragma region LifeTimePart

//BaseObject生命周期
UENUM()
enum class EBaseObjectLife : uint8
{
	None = 0, //初始化
	Init,
	Loading,
	Register,
	Enable,
	Disable,
	UnRegister,
	UnLoading
};

//BaseObject生命周期状态
UENUM()
enum class EBaseObjectState : uint8
{
	Active = 0, //激活
	Stable,		//稳定
	Destroy		//销毁
};
#pragma endregion

#pragma region ReflectPart

//对象调用协议
UENUM()
enum class EAgreementType : uint8
{
	SelfObject,			//给传入的对象通信
	OtherObject,		//给传入的对象之外的对象通信
	ClassOtherObject,	//给传入的对象的相同类的其他对象通信，调用这个方法要确保传过来的对象都是同一类的，如果不同类就多次通信
	SelfClass,			//给这个类的对象通信
	OtherClass,			//给这个类之外的类通信
	All					//给所有的对象通信
};


#pragma endregion 