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
			"EnhancedInput",		// Modern input system (UE5 default)
			"AIModule",				// AAIController, UAIPerceptionComponent, Blackboard
			"GameplayTasks",		// Required by behavior tree tasks
			"NavigationSystem",		// Enemy pathfinding
			"UMG"					// HUD / widget interaction from C++
		});
	}
}