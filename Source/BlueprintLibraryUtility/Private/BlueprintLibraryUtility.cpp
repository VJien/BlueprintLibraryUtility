// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BlueprintLibraryUtility.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FBlueprintLibraryUtilityModule"

void FBlueprintLibraryUtilityModule::StartupModule()
{
#if PLATFORM_64BITS
	FString platform = TEXT("Win64");
#else
	FString platform = TEXT("Win32");
#endif
	FString path = IPluginManager::Get().FindPlugin("BlueprintLibraryUtility")->GetBaseDir();
	FString dllpath = path + "BlueprintLibraryUtility/Source/ThirdParty/assimp/bin/" + platform+ "assimp-vc140-mt.dll";
	auto PdfDllHandle = FPlatformProcess::GetDllHandle(*dllpath);
	if (!PdfDllHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load PDF library."));
	}
}

void FBlueprintLibraryUtilityModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintLibraryUtilityModule, BlueprintLibraryUtility)