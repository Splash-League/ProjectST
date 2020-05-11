// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectST : ModuleRules
{
	public ProjectST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
