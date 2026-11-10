using UnrealBuildTool;
using System.Collections.Generic;

public class MoonKnightRPGEditorTarget : TargetRules
{
	public MoonKnightRPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("MoonKnightRPG");
	}
}
