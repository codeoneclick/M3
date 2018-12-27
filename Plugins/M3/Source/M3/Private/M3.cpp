// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3.h"
#include "M3EdMode.h"
#include "M3EdModeStyle.h"

#if WITH_EDITOR

#include "EditorStyleSet.h"

#endif
 
void M3Impl::StartupModule()
{
#if WITH_EDITOR

	FM3EdModeStyle::Startup();
	FEditorModeRegistry::Get().RegisterMode<FM3EdMode>(FM3EdMode::EM_M3, NSLOCTEXT("EditorModes", "M3EdMode", "M3"), FSlateIcon(FM3EdModeStyle::GetSlateStyle()->GetStyleSetName(), "M3EdModeMainIcon", "M3EdModeMainIcon.Small"), true, 400);

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
