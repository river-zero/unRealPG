using UnrealBuildTool;

public class For_The_Job : ModuleRules
{
	public For_The_Job(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", 
			"EnhancedInput", 
			"GeometryCollectionEngine" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
