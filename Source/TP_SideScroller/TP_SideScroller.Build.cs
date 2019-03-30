// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TP_SideScroller : ModuleRules
{
	public TP_SideScroller(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "TakahanoParis" }); // BaseCode
        PrivateDependencyModuleNames.AddRange(new string[] { "TakahanoParis" }); 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings", "InputCore" });

        PublicDependencyModuleNames.Add("Paper2D");
        PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer
        //PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"}); // video support

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");


        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "TP_SideScroller/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "TP_SideScroller/Private" });
    }
}

