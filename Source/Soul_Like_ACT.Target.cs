// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Soul_Like_ACTTarget : TargetRules
{
    public Soul_Like_ACTTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        ExtraModuleNames.Add("Soul_Like_ACT");
    }
}