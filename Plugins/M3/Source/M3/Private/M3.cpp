// Copyright serhii serhiiv 2017. All rights reserved.

#include "M3.h"
#include "M3EdMode.h"
#include "M3EdModeStyle.h"

#if WITH_EDITOR

#include "Editor/EditorStyle/Public/EditorStyleSet.h"

#endif
 
void M3Impl::StartupModule()
{
#if WITH_EDITOR

	FM3EdModeStyle::Startup();
	FEditorModeRegistry::Get().RegisterMode<FM3EdMode>(FM3EdMode::EM_M3, NSLOCTEXT("EditorModes", "M3EdMode", "M3"), FSlateIcon(FM3EdModeStyle::Get()->GetStyleSetName(), "LevelEditor.M3Mode", "LevelEditor.M3Mode.Small"), true, 400);

#endif
}
 
void M3Impl::ShutdownModule()
{
#if WITH_EDITOR

	FM3EdModeStyle::Shutdown();
	FEditorModeRegistry::Get().UnregisterMode(FM3EdMode::EM_M3);

#endif
}
 
IMPLEMENT_MODULE(M3Impl, M3)
