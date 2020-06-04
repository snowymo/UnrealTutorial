// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "VoIPAvatarsModule.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Features/IModularFeatures.h"
#include "Interfaces/IPluginManager.h"
//#include "OvrAvatarManager.h"

#define LOCTEXT_NAMESPACE "VoIPAvatarsModule"

void VoIPAvatarsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("VoIPAvatars")->GetBaseDir();

	
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
	//FOvrAvatarManager::Get().InitializeSDK();
}

void VoIPAvatarsModule::ShutdownModule()
{
	//FOvrAvatarManager::Get().ShutdownSDK();
	//FOvrAvatarManager::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(VoIPAvatarsModule, VoIPAvatars)
