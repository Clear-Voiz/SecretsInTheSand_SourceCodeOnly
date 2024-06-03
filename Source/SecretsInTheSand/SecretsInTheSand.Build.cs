// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SecretsInTheSand : ModuleRules
{
	public SecretsInTheSand(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam", "UMG","HairStrandsCore", "Niagara", "LevelSequence", "MovieScene", "AIModule", "SlateCore", "Slate", "NiagaraUIRenderer", "MediaAssets"});
	}
}
