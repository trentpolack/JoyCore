// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

// UBT.
using UnrealBuildTool;

/**
 * JoyCore Module Class Definition.
 *	Defines settings and dependencies for JoyCore.
 */
public class JoyCore : ModuleRules
{
	// Plugin settings used for the module rules and provide preprocessor defines as-needed.
	private const bool kJoyCoreSettings_WithCommonUI = true;
	
	// Module build target setup.
	public JoyCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Public include path list.
		PublicIncludePaths.AddRange(
			new string[]
			{
			}
			);
		
		// Private include path list.
		PrivateIncludePaths.AddRange(
			new string[]
			{
			}
			);
			
		// Public module dependency list.
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"Engine",
				"GameplayTags",
				"AssetRegistry",
				"StructUtils",
			}
			);
			
		// Private module dependency list.
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
			}
			);
		
		// Dynamically-loaded module list.
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
		
		// Add CommonUI modules and definitions (if needed).
		if(kJoyCoreSettings_WithCommonUI)
		{
			PublicDependencyModuleNames.Add("CommonUI");
			PublicDefinitions.Add("JOYCORE_WITH_COMMONUI=1");
		}
	}
}
