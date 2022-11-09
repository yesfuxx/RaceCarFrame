// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RaceCarFrame : ModuleRules
{
	public RaceCarFrame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysXVehicles", "HeadMountedDisplay", "DataDriven" });

		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
	}
}
