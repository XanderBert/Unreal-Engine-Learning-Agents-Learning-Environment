// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LA_TestEnv : ModuleRules
{
	public LA_TestEnv(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "LearningAgentsTraining", "ChaosVehicles" });
		PublicDependencyModuleNames.AddRange(new string[] {  "LearningAgents", "LearningAgentsTraining"  });
		
        
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});
		//PrivateDependencyModuleNames.AddRange(new string[] {  });

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
