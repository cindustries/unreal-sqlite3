// Copyright (c) 2015 Jussi Saarivirta 2016 conflict.industries MIT License (MIT)

using UnrealBuildTool;

public class CISQLite3 : ModuleRules
{
  public CISQLite3(TargetInfo Target)
  {
    
    PublicIncludePaths.AddRange(
      new string[] {
        "CISQLite3/Public"
      }
    );

    PrivateIncludePaths.AddRange(
      new string[] {
        "CISQLite3/Private"
      }
    );

    PublicDependencyModuleNames.AddRange(
      new string[] {
        "Engine",
        "Core",
        "CoreUObject"
      }
    );

    PrivateDependencyModuleNames.AddRange(
      new string[] {}
    );

    DynamicallyLoadedModuleNames.AddRange(
      new string[] {}
    );
  }
}
