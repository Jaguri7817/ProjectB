// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class ProjectB : ModuleRules
{
	public ProjectB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            
        });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectB",
			"ProjectB/Variant_Platforming",
			"ProjectB/Variant_Platforming/Animation",
			"ProjectB/Variant_Combat",
			"ProjectB/Variant_Combat/AI",
			"ProjectB/Variant_Combat/Animation",
			"ProjectB/Variant_Combat/Gameplay",
			"ProjectB/Variant_Combat/Interfaces",
			"ProjectB/Variant_Combat/UI",
			"ProjectB/Variant_SideScrolling",
			"ProjectB/Variant_SideScrolling/AI",
			"ProjectB/Variant_SideScrolling/Gameplay",
			"ProjectB/Variant_SideScrolling/Interfaces",
			"ProjectB/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
