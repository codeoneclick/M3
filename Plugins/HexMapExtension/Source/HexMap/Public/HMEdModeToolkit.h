// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"
#include "IDetailsView.h"
#include "M3Scheme.h"

class FHMEdModeToolkit : public FModeToolkit
{
private:

	static TSharedRef<SWidget> MAKE_CreateBoard_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_BoardSchemeSave_BTN(const FText& Label);

	static TSharedRef<SWidget> MAKE_SetTileSize_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddCircle_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddRectangle_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddTile_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_TilesBatchApplier_BTN(const FText& Label);

	static TSharedRef<SWidget> MAKE_CreateBoard_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_BoardScheme_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_BoardElements_SLOT(FHMEdModeToolkit* SELF);

	static TSharedRef<SWidget> MAKE_SetTileSize_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddCircle_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddRectangle_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddTile_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_TilesBatchApplier_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_Copyright_SLOT(FHMEdModeToolkit* SELF);

	static ECheckBoxState ON_Element_CHECKED(int ElementId);

	static FReply ON_CreateBoard_BTN();

	static FReply ON_SetTileSize_BTN();
	static FReply ON_AddCircle_BTN();
	static FReply ON_AddRectangle_BTN();
	static FReply ON_AddTile_BTN();
	static FReply ON_RandomizeTiles_BTN();
	static FReply ON_TilesBatchApplier_BTN();

	static EM3ElementId SelectedElementId;

protected:

	static void GetSelectedTiles(TArray<class AHMTile*>& Tiles);

	TSharedPtr<IDetailsView> EdModePanel_CreateBoard;
	TSharedPtr<IDetailsView> EdModePanel_BoardScheme;

	TSharedPtr<IDetailsView> EdModePanelSetTileSize;
	TSharedPtr<IDetailsView> EdModePanelAddCircle;
	TSharedPtr<IDetailsView> EdModePanelAddRectangle;
	TSharedPtr<IDetailsView> EdModePanelAddTile;
	TSharedPtr<IDetailsView> EdModePanelRandomizeTiles;
	TSharedPtr<IDetailsView> EdModePanelTilesBatchApplier;
	TSharedPtr<SWidget> ToolkitWidget;

	static class AM3App* GetM3App();
	static void GenerateElements(class AM3App* M3App);

public:

	FHMEdModeToolkit() = default;
	~FHMEdModeToolkit() = default;

	virtual void Init(const TSharedPtr<class IToolkitHost>& ToolkitHost) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
};

#endif