// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OpenWorldStarterServerTarget : TargetRules
{
    public OpenWorldStarterServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        ExtraModuleNames.AddRange(new string[] { "OpenWorldStarter" });
    }
}