// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TP_UserInterface : ModuleRules
{
	public TP_UserInterface(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings"});
        PublicDependencyModuleNames.AddRange(new string[] { "UMG", "StreamingPauseRendering" });

        PublicDependencyModuleNames.AddRange(new string[] { "TakahanoParis" }); // Third Person Gameplay

        //PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer
        PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"}); // video support

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        //PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "TP_UserInterface/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "TP_UserInterface/Private" });
    }
}

