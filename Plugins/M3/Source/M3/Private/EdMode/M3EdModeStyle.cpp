// Copyright serhii serhiiv 2017. All rights reserved.

#include "M3EdModeStyle.h"

#if WITH_EDITOR

#include "SlateStyle.h"
#include "IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define ICON_TAB_MODE( RelativePath, ... ) FSlateImageBrush( FM3EdModeStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )

FString FM3EdModeStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("M3"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FM3EdModeStyle::StyleSet = NULL;
TSharedPtr< class ISlateStyle > FM3EdModeStyle::Get() { return StyleSet; }

void FM3EdModeStyle::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing M3 Style"));

	const FVector2D Icon40x40(40.0f, 40.0f);

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable<FSlateStyleSet>(new FSlateStyleSet("M3EdStyle"));

	{
		StyleSet->Set("LevelEditor.M3Mode", new ICON_TAB_MODE("icon_M3_Mode_Selected_40px", Icon40x40));
		StyleSet->Set("LevelEditor.M3Mode.Small", new ICON_TAB_MODE("icon_M3_Mode_Selected_40px", Icon40x40));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	UE_LOG(LogTemp, Log, TEXT("Finished Initializing M3 Style"));
};

#undef ICON_TAB_MODE

void FM3EdModeStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#endif
