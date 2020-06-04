// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class ShooterGame_PS4 : ShooterGame
{
    public ShooterGame_PS4(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDefinitions.Add("SHOOTER_CONSOLE_UI=1");
    }
}
