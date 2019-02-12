// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP_ThirdPerson : ModuleRules
{
    public TP_ThirdPerson(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //Public module names that this module uses.
        //In case you would like to add various classes that you're going to use in your game
        //you should add the core,coreuobject and engine dependencies.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "TakahanoParis" });

        //PublicDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay" });

        //PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");


        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "TP_ThirdPerson/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "TP_ThirdPerson/Private" });
    }
}
