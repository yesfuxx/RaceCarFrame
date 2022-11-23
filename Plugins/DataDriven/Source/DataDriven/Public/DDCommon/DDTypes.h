// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/GameEngine.h"
#include "DDTypes.generated.h"

// enum class EAgreementType : uint8;
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

//调用结果，项目开发时请确保每次都能调用成功
UENUM()
enum class ECallResult : uint8
{
	NoModule = 0,	//缺失模组
	LackObject,		//缺失部分对象
	NoFunction,		//缺失方法
	Succeed			//调用成功
};

//通信参数结构体基类
struct DDParam
{
public:

	//调用结果
	ECallResult CallResult;

	//参数指针
	void* ParamPtr;
};

//通信协议，Module方法
struct DDModuleAgreement
{
public:

	//模组ID
	int32 ModuleIndex;

	//方法名
	FName FunctionName;
	
};

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

//通信协议，DDOO方法
struct DDObjectAgreement
{
public:

	//模组ID
	int32 ModuleIndex;

	//协议类型
	EAgreementType AgreementType;

	//对象组名
	TArray<FName> ObjectGroup;

	//方法名
	FName FunctionName;
};

#pragma endregion


#pragma region DDAnyFun

//存储任意类型方法结构体
struct DDAnyFun
{
	struct BaseFun
	{
	public:
		virtual ~BaseFun() {};
	};
	template<typename RetType, typename... VarTypes>
	struct ValFun : public BaseFun
	{
	public:
		TFunction<RetType(VarTypes...)> TarFun;
		ValFun(const TFunction<RetType(VarTypes...)> InsFun) : TarFun(InsFun) {}
		RetType Execute(VarTypes... Params)
		{
			return TarFun(Params...);
		}
	};
	BaseFun* FunPtr;
public:
	DDAnyFun() : FunPtr(nullptr) {}
	template<typename RetType, typename... VarTypes>
	DDAnyFun(const TFunction<RetType(VarTypes...)> InsFun) : FunPtr(new ValFun<RetType, VarTypes...>(InsFun)) {}
	~DDAnyFun() { delete FunPtr; }
	template<typename RetType, typename... VarTypes>
	RetType Execute(VarTypes... Params)
	{
		ValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr->Execute(Params...);
	}
	template<typename RetType, typename... VarTypes>
	TFunction<RetType(VarTypes...)>& GetFun()
	{
		ValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr->TarFun;
	}
};

#pragma endregion

#pragma region  aaa

// //存储任意类型的数据结构
// struct AnyElement
// {
// 	//元素父结构体
// 	struct BaseElement
// 	{
// 	public:
// 		virtual ~BaseElement(){}
// 	};
// 	//实际存储值的结构体
// 	template<typename T>
// 	struct ValueElement : public BaseElement
// 	{
// 	public:
// 		T Value;
// 		ValueElement(const T& InValue) : Value(InValue) {}
// 	};
// 	//父结构体指针，用于存储实例化的子结构体的地址
// 	BaseElement* ElementPtr;
// public:
// 	AnyElement() : ElementPtr(nullptr) {}
// 	//构造函数传入值并且实例化子结构体存储于父结构体指针
// 	template<typename T>
// 	AnyElement(const T& InValue) : ElementPtr(new ValueElement<T>(InValue)) {}
// 	~AnyElement() { delete ElementPtr; }
// 	//获取保存的变量
// 	template<typename T>
// 	T& Get()
// 	{
// 		//通过将父类指针强转为子类指针来获取子类指针内保存的值
// 		ValueElement<T>* SubPtr = static_cast<ValueElement<T>*>(ElementPtr);
// 		return SubPtr->Value;
// 	}
// };
#pragma endregion


#pragma region DDMsgNode

//事件节点
struct DDMsgNode
{
	//被调用的接口数量
	int32 CallCount;
	//方法列表
	TMap<int32, DDAnyFun*> FunQuene;
	//注册方法
	template<typename RetType, typename... VarTypes>
	int32 RegisterFun(TFunction<RetType(VarTypes...)> InsFun);
	//注销方法
	void UnRegisterFun(int32 FunID)
	{
		//从列表移除对象
		DDAnyFun* DesPtr = * FunQuene.Find(FunID);
		FunQuene.Remove(FunID);
		delete DesPtr;
	}
	//执行方法
	template<typename RetType, typename... VarTypes>
	RetType Execute(VarTypes... Params);
	//判断是否有绑定的函数
	bool IsBound() { return FunQuene.Num() > 0; }
	//如果有绑定函数就去执行
	template<typename RetType, typename... VarTypes>
	bool ExecuteIfBound(VarTypes... Params);
	//构造函数,初始化CallCount为0
	DDMsgNode() : CallCount(0){}
};

template <typename RetType, typename... VarTypes>
int32 DDMsgNode::RegisterFun(TFunction<RetType(VarTypes...)> InsFun)
{
	//获取方法序列里的所有下标
	TArray<int32> FunKeyQuene;
	FunQuene.GenerateKeyArray(FunKeyQuene);
	//获取新下标
	int32 NewID;
	// for (int32 i = FunKeyQuene)
	
}

template <typename RetType, typename... VarTypes>
RetType DDMsgNode::Execute(VarTypes... Params)
{
	//
}

#pragma endregion 
