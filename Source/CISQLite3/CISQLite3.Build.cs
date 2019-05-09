// Copyright (c) 2015 Jussi Saarivirta 2016 conflict.industries MIT License (MIT)

using UnrealBuildTool;
using System.IO;

public class CISQLite3 : ModuleRules
{
  public CISQLite3(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
    PrivateIncludePaths.AddRange(new string[] {  Path.Combine(ModuleDirectory, "Private") });

    PrivatePCHHeaderFile = "Private/CISQLite3PrivatePCH.h";

    PublicDependencyModuleNames.AddRange(
      new string[] {
        "Engine",
        "Core",
        "CoreUObject"
      }
    );

    PrivateDefinitions.Add("SQLITE_ENABLE_DESERIALIZE=1");

    PrivateDependencyModuleNames.AddRange(
      new string[] {}
    );

    DynamicallyLoadedModuleNames.AddRange(
      new string[] {}
    );
  }
}
