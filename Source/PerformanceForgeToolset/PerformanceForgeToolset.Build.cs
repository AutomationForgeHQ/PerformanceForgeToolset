using UnrealBuildTool;

public class PerformanceForgeToolset : ModuleRules
{
	public PerformanceForgeToolset(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"ToolsetRegistry",     // UToolsetDefinition is a public base class
				"PerformanceForge",    // the capability this exposes
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",         // IPluginManager, so GetToolsetVersion() reads the descriptor
				"UnrealEd",   // RaiseScriptError comes from KismetSystemLibrary
				"Slate",      // invoking the capture tab when a session starts
				"SlateCore",
			}
			);

		// As with every other toolset in this family: no dependency on ModelContextProtocol. Tools
		// register with ToolsetRegistry and MCP picks them up from there.
	}
}
