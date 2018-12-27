// Copyright serhii serhiiv 2018. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class M3 : ModuleRules
{
    public M3(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Core"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Core/KVO"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Core/MVC"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/EdMode"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay/Components"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay/MVC"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay/MVC/Controllers"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay/MVC/Models"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay/MVC/Views"));

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Core"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Core/KVO"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Core/MVC"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/EdMode"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay/Components"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay/MVC"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay/MVC/Controllers"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay/MVC/Models"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Gameplay/MVC/Views"));


        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "RenderCore"
        });

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "InputCore",
                "SlateCore",
                "Slate",
                "Projects",
                "RenderCore",
                "ShaderCore",
                "RHI",
                "AIModule",
                "PropertyEditor",
                "WorkspaceMenuStructure",
                "UnrealEd",
                "LevelEditor",
                "EditorStyle",
                "ContentBrowser"
            });
        }
    }
}