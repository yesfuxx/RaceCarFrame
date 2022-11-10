// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "LifeCallActor.generated.h"

/**
 * 
 */
UCLASS()
class RACECARFRAME_API ALifeCallActor : public ADDActor
{
	GENERATED_BODY()

public:

	virtual void DDInit() override;

	virtual void DDLoading() override;

	virtual void DDRegister() override;

	virtual void DDEnable() override;

	virtual void DDTick(float DeltaSeconds) override;
	
};
