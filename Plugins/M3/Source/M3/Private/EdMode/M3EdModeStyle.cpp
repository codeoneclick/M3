// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3EdModeStyle.h"

#if WITH_EDITOR

#include "SlateStyle.h"
#include "SlateGameResources.h"
#include "IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define ICON_TAB_MODE(RelativePath, ...) FSlateImageBrush( FM3EdModeStyle::InContent(RelativePath, ".png" ), __VA_ARGS__ )

FString FM3EdModeStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension) {
	static FString ContentPath = IPluginManager::Get().FindPlugin(TEXT("M3"))->GetContentDir();
	return (ContentPath / RelativePath) + Extension;
}

TSharedPtr<FSlateStyleSet> FM3EdModeStyle::StyleSet = nullptr;
TSharedPtr<class ISlateStyle> FM3EdModeStyle::GetSlateStyle() { return StyleSet; }

void FM3EdModeStyle::Startup() {
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);
	if (StyleSet.IsValid()) {
		return;
	}

	StyleSet = MakeShareable<FSlateStyleSet>(new FSlateStyleSet("M3EdStyle"));
	{
		StyleSet->Set("M3EdModeMainIcon", new ICON_TAB_MODE("Icons/M3EdModeMainIcon", Icon40x40));
		StyleSet->Set("M3EdModeMainIcon.Small", new ICON_TAB_MODE("Icons/M3EdModeMainIcon", Icon40x40));
		StyleSet->Set("M3EdModeIcon.SuperelementMatch4", new ICON_TAB_MODE("Icons/M3EdModeIconSuperElementMatch4", Icon64x64));
		StyleSet->Set("M3EdModeIcon.SuperelementMatch5", new ICON_TAB_MODE("Icons/M3EdModeIconSuperElementMatch5", Icon64x64));
		StyleSet->Set("M3EdModeIcon.SuperelementMatch6", new ICON_TAB_MODE("Icons/M3EdModeIconSuperElementMatch6", Icon64x64));
		StyleSet->Set("M3EdModeIcon.SuperelementMatch7", new ICON_TAB_MODE("Icons/M3EdModeIconSuperElementMatch7", Icon64x64));
	}
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

#undef ICON_TAB_MODE

void FM3EdModeStyle::Shutdown() {
	if (StyleSet.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#endif
