// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

// UBT.
using UnrealBuildTool;

/**
 * JoyCoreEditor Module Class Definition.
 *	Defines settings and dependencies for JoyCoreEditor.
 */
public class JoyCoreEditor : ModuleRules
{
    // Plugin settings used for the module rules and provide preprocessor defines as-needed.
    private const bool kJoyCoreEditorSettings_WithCommonUI = true;

    public JoyCoreEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public module dependency list.
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

		// Private module dependency list.
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
        
        // Add CommonUI modules and definitions (if needed).
        if(kJoyCoreEditorSettings_WithCommonUI)
        {
            PublicDependencyModuleNames.Add("CommonUI");
            PublicDefinitions.Add("JOYCOREEDITOR_WITH_COMMONUI=1");
        }
    }
}