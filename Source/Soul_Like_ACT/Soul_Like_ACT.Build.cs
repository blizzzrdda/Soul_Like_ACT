// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Soul_Like_ACT : ModuleRules
{
    public Soul_Like_ACT(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Slate",
            "SlateCore",
            "Engine",
            "InputCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "UMG",
            "NavigationSystem",
            "InputBuffer",
            "BlueprintJson",
            "Json",
            "UINavigation"
        });
    }
}