// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TP_ThirdPerson : ModuleRules
{
	public TP_ThirdPerson(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "AIModule" });

        PublicDependencyModuleNames.AddRange(new string[] { "TakahanoParis" }); // Third Person Gameplay

        PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");


        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "TP_ThirdPerson/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "TP_ThirdPerson/Private" });
    }
}

