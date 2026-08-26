using UnrealBuildTool;
 
public class MoonKnightRPG : ModuleRules
{
	public MoonKnightRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",		
			"AIModule",				
			"GameplayTasks",		
			"NavigationSystem",		
			"UMG"					
		});
	}
}