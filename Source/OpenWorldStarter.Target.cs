// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class OpenWorldStarterTarget : TargetRules
{
	public OpenWorldStarterTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.AddRange(new string[] { "OpenWorldStarter" });
    }
}
