// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "RaceCarFrameHud.generated.h"


UCLASS(config = Game)
class ARaceCarFrameHud : public AHUD
{
	GENERATED_BODY()

public:
	ARaceCarFrameHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
