// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class BlueprintLibraryUtility : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
    }

    public BlueprintLibraryUtility(ReadOnlyTargetRules Target) : base(Target)
	{
	    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
                Path.Combine(ThirdPartyPath, "assimp/include")
				// ... add public include paths required here ...
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"BlueprintLibraryUtility/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                
                // ... add other public dependencies that you statically link with here ...
			}
			);


        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"ImageWrapper",
                "Engine",
                "RHI",
                "RenderCore",
                "ProceduralMeshComponent",
				"UMG",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
            }
            );

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32";
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp/lib",PlatformString, "assimp-vc140-mt.lib"));

            RuntimeDependencies.Add(Path.Combine(ThirdPartyPath, "assimp/bin",PlatformString, "assimp-vc140-mt.dll"));
        }
		else if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			string PlatformString =  "Mac";
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp/lib",PlatformString, "libassimp.4.1.0.dylib"));
		}
    }
}
