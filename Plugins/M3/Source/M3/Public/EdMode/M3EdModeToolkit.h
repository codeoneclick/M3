// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"
#include "IDetailsView.h"
#include "M3Scheme.h"

class FM3EdModeToolkit : public FModeToolkit
{
private:

	static TSharedRef<SWidget> MAKE_BoardCreate_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_BoardReskin_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_BoardSchemeSave_BTN(const FText& Label);

	static TSharedRef<SWidget> MAKE_BoardCreate_SLOT(FM3EdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_BoardScheme_SLOT(FM3EdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_BoardElements_SLOT(FM3EdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_BoardReskin_SLOT(FM3EdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_Copyright_SLOT(FM3EdModeToolkit* SELF);

	static ECheckBoxState ON_Element_CHECKED(int ElementId);

	static FReply ON_BoardCreate_BTN();
	static FReply ON_BoardReskin_BTN();
	static FReply ON_BoardSchemeSave_BTN();

	static EM3ElementId SelectedElementId;

protected:

	TSharedPtr<IDetailsView> EdModePanel_BoardCreate;
	TSharedPtr<IDetailsView> EdModePanel_BoardScheme;
	TSharedPtr<IDetailsView> EdModePanel_BoardReskin;

	TSharedPtr<SWidget> ToolkitWidget;

	static class AM3App* GetM3App();
	static void GenerateElementsScheme(class AM3App* M3App);
	static void GenerateElementsVisual(class AM3App* M3App);

public:

	FM3EdModeToolkit() = default;
	~FM3EdModeToolkit() = default;

	virtual void Init(const TSharedPtr<class IToolkitHost>& ToolkitHost) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
};

#endif