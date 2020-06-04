// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System;
using System.IO;

public class VoIP422 : ModuleRules
{
	public VoIP422(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // List of public dependency module names (no path needed) (automatically does the private/public include).
        //These are modules that are required by our public source files
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Voice" });

        PrivateDependencyModuleNames.AddRange(new string[] { "VoIP422Avatars", "OVRLipSync" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
