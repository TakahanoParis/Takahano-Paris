// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TakahanoParis : ModuleRules
{
	public TakahanoParis(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "UMG", "StreamingPauseRendering", "AIModule" });

        PublicDependencyModuleNames.AddRange(new string[] { "TP_ThirdPerson" }); // Third Person Gameplay
        PublicDependencyModuleNames.AddRange(new string[] { "TP_TopDown" }); // Top Down Gameplay
        PublicDependencyModuleNames.AddRange(new string[] { "TP_SideScroller" }); // Side Scroller Gameplay
        //PublicDependencyModuleNames.AddRange(new string[] { "TP_UserInterface" }); // UserInterface Classes Gameplay

        PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer
        PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"}); // video support




        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}

