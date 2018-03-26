// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "ISettingsModule.h"
#include "ModuleManager.h"

class FM3EdModeStyle
{
protected:

	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);
	static TSharedPtr< class FSlateStyleSet > StyleSet;

public:

	FM3EdModeStyle() = default;
	~FM3EdModeStyle() = default;

	static void Startup();
	static void Shutdown();
	static TSharedPtr<class ISlateStyle> Get();
};

#endif
