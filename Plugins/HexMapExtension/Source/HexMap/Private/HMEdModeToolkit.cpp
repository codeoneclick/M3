// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMEdModeToolkit.h"

#if WITH_EDITOR

#include "HMEdMode.h"
#include "Engine.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SExpandableArea.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "HMEdModeProperties.h"
#include "EditorViewportClient.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"
#include "HMTileRandomizer.h"
#include "HMTileBatchApplier.h"
#include "M3App.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"

EM3ElementId FHMEdModeToolkit::SelectedElementId = EM3ElementId::UNKNOWN;

#define LOCTEXT_NAMESPACE "HMEdModeToolkit"

void FHMEdModeToolkit::Init(const TSharedPtr<class IToolkitHost>& ToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return true;
		}
	};

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Left)
		.Padding(16)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_CreateBoard_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_BoardScheme_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_BoardElements_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_AddRectangle_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_AddTile_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_RandomizeTiles_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_TilesBatchApplier_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Center)
			[
				MAKE_Copyright_SLOT(this)
			]
		];
	FModeToolkit::Init(ToolkitHost);

	UWorld* World = GEditor->GetEditorWorldContext().World();
	AM3App* M3App = nullptr;
	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		M3App = static_cast<AM3App*>(*It);
		break;
	}

	if (M3App) {
		FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
		if (HexMapEdMode->EdModeProps_BoardScheme && M3App->BoardScheme) {
			HexMapEdMode->EdModeProps_BoardScheme->RedElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_RED);
			HexMapEdMode->EdModeProps_BoardScheme->GreenElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_GREEN);
			HexMapEdMode->EdModeProps_BoardScheme->BlueElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_BLUE);
			HexMapEdMode->EdModeProps_BoardScheme->YellowElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_YELLOW);
			HexMapEdMode->EdModeProps_BoardScheme->OrangeElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_ORANGE);
			HexMapEdMode->EdModeProps_BoardScheme->PurpleElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_PURPLE);
		}
	}
}

FName FHMEdModeToolkit::GetToolkitFName() const
{
	return FName("HMEdModeToolkit");
}

FText FHMEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("HMEdModeToolkit", "DisplayName", "HexMapEd Tool");
}

class FEdMode* FHMEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_Copyright_SLOT(FHMEdModeToolkit* SELF)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(32.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.AutoWrapText(true)
			.Text(LOCTEXT("CopyrightLabel", "Copyright (c) 2017 Serhii Serhiiv. All rights reserved."))
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_CreateBoard_SLOT(FHMEdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_CreateBoard = PropEdModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanel_CreateBoard->SetObject(HexMapEdMode->EdModeProps_CreateBoard, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("M3CreateBoardHeader", "M3CreateBoardHeader", "Create Board"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
			[
				SELF->EdModePanel_CreateBoard.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				MAKE_CreateBoard_BTN(LOCTEXT("M3CreateBoardBTN", "Create"))
			]
		]
	];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardScheme_SLOT(FHMEdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardScheme = PropEdModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanel_BoardScheme->SetObject(HexMapEdMode->EdModeProps_BoardScheme, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("M3BoardSchemeHeader", "M3BoardSchemeHeader", "Board Scheme"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanel_BoardScheme.ToSharedRef()
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardElements_SLOT(FHMEdModeToolkit* SELF) {

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("M3BoardElementsHeader", "M3BoardElementsHeader", "Board Elements"))
			]
			.BodyContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoWidth()
				.Padding(8.f)
				[
					SNew(SWrapBox)
					.PreferredWidth(100.f)
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Top)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_RED))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_RED;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementRedScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor::Red)
							]
						]
					]
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Bottom)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_GREEN))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_GREEN;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementGreenScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor::Green)
							]
						]
					]
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Bottom)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_BLUE))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_BLUE;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementBlueScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor::Blue)
							]
						]
					]
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Bottom)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_YELLOW))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_YELLOW;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementYellowScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor::Yellow)
							]
						]
					]
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Bottom)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_ORANGE))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_ORANGE;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementOrangeScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor(1.f, .5f, 0.f, 1.f))
							]
						]
					]
					+ SWrapBox::Slot()
					.Padding(5)
					.VAlign(VAlign_Bottom)
					[
						SNew(SCheckBox)
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::ELEMENT_PURPLE))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::ELEMENT_PURPLE;
								AM3CellScheme::EdModeSelectedAppointmentScheme = GetM3App()->BoardScheme->ElementPurpleScheme;
							}
						})
						.Content()
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(64)
							.HeightOverride(64)
							.Padding(15)
							[
								SNew(SImage)
								.ColorAndOpacity(FLinearColor(.5f, .0f, .5f, 1.f))
							]
						]
					]
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_SetTileSize_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelSetTileSize = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelSetTileSize->SetObject(HexMapEdMode->EdModePropertiesSetTileSize, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("SetHexMapTileSizeHeader", "SetHexMapTileSizeHeader", "Set Tile Size"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelSetTileSize.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_SetTileSize_BTN(LOCTEXT("SetHMTileSizeBTN", "Set Size"))
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddCircle_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddCircle = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddCircle->SetObject(HexMapEdMode->EdModePropertiesAddCircle, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("HM_HEADER_AddCircle", "HM_HEADER_AddCircle", "Add Circle"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddCircle.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddCircle_BTN(LOCTEXT("HM_BTN_AddCircle", "Add Circle"))
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddRectangle_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddRectangle = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddRectangle->SetObject(HexMapEdMode->EdModePropertiesAddRectangle, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("HM_HEADER_AddRectangle", "HM_HEADER_AddRectangle", "Add Rectangle"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddRectangle.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddRectangle_BTN(LOCTEXT("HM_BTN_AddRectangle", "Add Rectangle"))
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddTile_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddTile = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddTile->SetObject(HexMapEdMode->EdModePropertiesAddTile, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("AddTileHeader", "AddTileHeader", "Add Tile"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddTile.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddTile_BTN(LOCTEXT("AddHMTileBTN", "Add Tile"))
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_RandomizeTiles_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelRandomizeTiles = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelRandomizeTiles->SetObject(HexMapEdMode->EdModePropertiesRandomizeTiles, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("RandomizeTilesHeader", "RandomizeTilesHeader", "Randomize Tiles"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
			[
				SELF->EdModePanelRandomizeTiles.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				MAKE_RandomizeTiles_BTN(LOCTEXT("RandomizeHMTilesBTN", "Randomize"))
			]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_TilesBatchApplier_SLOT(FHMEdModeToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelTilesBatchApplier = PropertyEditorModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelTilesBatchApplier->SetObject(HexMapEdMode->EdModePropertiesTileBatchApplier, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("TilesBatchApplierHeader", "TilesBatchApplierHeader", "Tiles Batch Applier"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelTilesBatchApplier.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_TilesBatchApplier_BTN(LOCTEXT("HMTilesBatchApplierBTN", "Apply"))
				]
			]
		];
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_SetTileSize_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_SetTileSize_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddCircle_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_AddCircle_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddRectangle_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_AddRectangle_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_AddTile_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_AddTile_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_RandomizeTiles_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_RandomizeTiles_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_TilesBatchApplier_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_TilesBatchApplier_BTN);
}

FReply FHMEdModeToolkit::ON_CreateBoard_BTN() {
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	if (!HexMapEdMode->EdModeProps_CreateBoard->AssetsBundle_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Assets Bundle should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->BoardScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Board Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->CellScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Cell Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->FunctionalSpawnScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Spawn Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementRedScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Red Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementGreenScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Green Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementBlueScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Blue Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementYellowScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Yellow Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementOrangeScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Orange Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_CreateBoard->ElementPurpleScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Purple Element Scheme should be assigned!");
		return FReply::Handled();
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_CREATE_BOARD", "M3_TRANSITION_CREATE_BOARD"));
	{
		AM3App* M3App = GetM3App();
		if (!M3App) {
			M3App = World->SpawnActor<AM3App>(FVector(0.f), FRotator(0.f));
		}

		M3App->AssetsBundle_BP = HexMapEdMode->EdModeProps_CreateBoard->AssetsBundle_BP;
		M3App->BoardScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->BoardScheme_BP;
		M3App->CellScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->CellScheme_BP;
		M3App->FunctionalSpawnScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->FunctionalSpawnScheme_BP;
		M3App->ElementRedScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementRedScheme_BP;
		M3App->ElementGreenScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementGreenScheme_BP;
		M3App->ElementBlueScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementBlueScheme_BP;
		M3App->ElementYellowScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementYellowScheme_BP;
		M3App->ElementOrangeScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementOrangeScheme_BP;
		M3App->ElementPurpleScheme_BP = HexMapEdMode->EdModeProps_CreateBoard->ElementPurpleScheme_BP;

		M3App->AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(M3App, M3App->AssetsBundle_BP));

		if (!M3App->BoardScheme) {
			M3App->BoardScheme = World->SpawnActor<AM3BoardScheme>(M3App->BoardScheme_BP);
			M3App->BoardScheme->AttachToActor(M3App, FAttachmentTransformRules::KeepWorldTransform);
		}
		M3App->BoardScheme->Cols = HexMapEdMode->EdModeProps_CreateBoard->Cols;
		M3App->BoardScheme->Rows = HexMapEdMode->EdModeProps_CreateBoard->Rows;

		if (!M3App->BoardScheme->ElementRedScheme) {
			M3App->BoardScheme->ElementRedScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementRedScheme_BP);
			M3App->BoardScheme->ElementRedScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementGreenScheme) {
			M3App->BoardScheme->ElementGreenScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementGreenScheme_BP);
			M3App->BoardScheme->ElementGreenScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementBlueScheme) {
			M3App->BoardScheme->ElementBlueScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementBlueScheme_BP);
			M3App->BoardScheme->ElementBlueScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementYellowScheme) {
			M3App->BoardScheme->ElementYellowScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementYellowScheme_BP);
			M3App->BoardScheme->ElementYellowScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementOrangeScheme) {
			M3App->BoardScheme->ElementOrangeScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementOrangeScheme_BP);
			M3App->BoardScheme->ElementOrangeScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementPurpleScheme) {
			M3App->BoardScheme->ElementPurpleScheme = World->SpawnActor<AM3CellAppointmentScheme>(M3App->ElementPurpleScheme_BP);
			M3App->BoardScheme->ElementPurpleScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		GenerateElements(M3App);
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_AddCircle_BTN()
{
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddCircle->Tile_BP)
	{
		FVector Location = HexMapEdMode->EdModePropertiesAddCircle->Location;
		if (Location.IsZero())
		{
			FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
			FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
			FVector EditorCameraPosition = ViewportClient->GetViewLocation();
			float Distance = 3000.f;
			Location = EditorCameraPosition + EditorCameraDirection * Distance;
		}

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_AddCircle", "HM_TRANSITION_AddCircle"));
		{
			GEditor->SelectNone(true, true);
			int32 Radius = HexMapEdMode->EdModePropertiesAddCircle->Radius;
			for (int32 Q = -Radius; Q <= Radius; ++Q)
			{
				int32 R1 = std::max(-Radius, -Q - Radius);
				int32 R2 = std::min(Radius, -Q + Radius);

				for (int32 R = R1; R <= R2; R++)
				{
					FHMCoord HexCoord = FHMCoord::Init(Q, R, -Q - R);
					FVector SnapLocation = FHMUtilities::ToSnapLocation(World, HexCoord);
					SnapLocation = FHMUtilities::ToSnapLocation(World, SnapLocation + Location);

					AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddCircle->Tile_BP, SnapLocation, FRotator(0.f));
					GEditor->SelectActor(Tile, true, true);
				}
			}
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}

	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_AddRectangle_BTN()
{
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddRectangle->Tile_BP)
	{
		FVector Location = HexMapEdMode->EdModePropertiesAddRectangle->Location;
		if (Location.IsZero())
		{
			FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
			FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
			FVector EditorCameraPosition = ViewportClient->GetViewLocation();
			float Distance = 3000.f;
			Location = EditorCameraPosition + EditorCameraDirection * Distance;
		}

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_AddRectangle", "HM_TRANSITION_AddRectangle"));
		{
			GEditor->SelectNone(true, true);
			int32 SizeX = HexMapEdMode->EdModePropertiesAddRectangle->SizeX;
			int32 SizeY = HexMapEdMode->EdModePropertiesAddRectangle->SizeY;
			for (int32 i = 0; i < SizeX; ++i)
			{
				for (int32 j = 0; j < SizeY; ++j)
				{
					FHMCoord HexCoord = FHMCoord::QOffsetToCube(FHMCoord::EHMDirection::ODD, FVector2D(i, j));
					FVector SnapLocation = FHMUtilities::ToSnapLocation(World, HexCoord);
					SnapLocation = FHMUtilities::ToSnapLocation(World, SnapLocation + Location);

					AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddRectangle->Tile_BP, SnapLocation, FRotator(0.f));
					GEditor->SelectActor(Tile, true, true);
				}
			}
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}

	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_AddTile_BTN()
{
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddTile->Tile_BP)
	{
		FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
		FVector EditorCameraPosition = ViewportClient->GetViewLocation();
		float Distance = 3000.f;
		FVector Location = EditorCameraPosition + EditorCameraDirection * Distance;

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("AddHMTile", "AddHMTile"));
		{
			GEditor->SelectNone(true, true);
			AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddTile->Tile_BP, Location, FRotator(0.f));
			GEditor->SelectActor(Tile, true, true);
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}
	
	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_RandomizeTiles_BTN()
{
	TArray<AHMTile*> SelectedTiles;
	GetSelectedTiles(SelectedTiles);
	if (SelectedTiles.Num() > 0)
	{
		FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModePropertiesRandomizeTiles->Randomizer_BP)
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_Randomize", "HM_TRANSITION_Randomize"));
			AHMTileRandomizer* Randomizer = World->SpawnActor<AHMTileRandomizer>(HexMapEdMode->EdModePropertiesRandomizeTiles->Randomizer_BP, FVector(0.f), FRotator(0.f));
			for (AHMTile* Tile : SelectedTiles)
			{
				Randomizer->Randomize(Tile);
			}
			Randomizer->Destroy();
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You need to choose Randomizer Blueprint at first!");
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one Tile should be selected!");
	}
	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_TilesBatchApplier_BTN()
{
	TArray<AHMTile*> SelectedTiles;
	GetSelectedTiles(SelectedTiles);
	if (SelectedTiles.Num() > 0)
	{
		FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModePropertiesTileBatchApplier->Applier_BP)
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			GEditor->BeginTransaction(LOCTEXT("HMTilesBatchApplier", "HMTilesBatchApplier"));
			AHMTileBatchApplier* Applier = World->SpawnActor<AHMTileBatchApplier>(HexMapEdMode->EdModePropertiesTileBatchApplier->Applier_BP, FVector(0.f), FRotator(0.f));
			for (AHMTile* Tile : SelectedTiles)
			{
				Applier->Apply(Tile);
			}
			Applier->Destroy();
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You need to choose Applier Blueprint at first!");
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one HexMapTile should be selected!");
	}
	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_SetTileSize_BTN()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	GEditor->BeginTransaction(LOCTEXT("SetHMTileSize", "SetHMTileSize"));
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	AHMGrid* Grid = FHMUtilities::GetGrid(World);
	Grid->OnTileSizeChanged(HexMapEdMode->EdModePropertiesSetTileSize->TileSize);
	GEditor->EndTransaction();
	return FReply::Handled();
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_CreateBoard_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_CreateBoard_BTN);
}

void FHMEdModeToolkit::GetSelectedTiles(TArray<class AHMTile*>& Tiles)
{
	USelection* Selection = GEditor->GetSelectedActors();
	for (int32 i = 0; i < Selection->Num(); ++i)
	{
		UObject* Object = Selection->GetSelectedObject(i);
		if (Object && Object->IsA(AHMTile::StaticClass()))
		{
			Tiles.Add(Cast<AHMTile>(Object));
		}
	}
}

ECheckBoxState FHMEdModeToolkit::ON_Element_CHECKED(int ElementId) {
	return ElementId == static_cast<int>(SelectedElementId) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

AM3App* FHMEdModeToolkit::GetM3App() {
	UWorld* World = GEditor->GetEditorWorldContext().World();
	AM3App* M3App = nullptr;
	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		M3App = static_cast<AM3App*>(*It);
		break;
	}
	return M3App;
}

void FHMEdModeToolkit::GenerateElements(class AM3App* M3App) {
	
	const auto World = GEditor->GetEditorWorldContext().World();
	const auto BoardScheme = M3App->BoardScheme;
	const auto BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(M3App->AssetsBundle);

	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	int Delta = BoardScheme->Cells.Num() - Cols * Rows;
	if (Delta > 0) {
		while (BoardScheme->Cells.Num() != Cols * Rows) {
			BoardScheme->Cells.Pop()->Destroy();
		}
	}

	BoardScheme->Cells.SetNum(Cols * Rows);
	for (int Col = 0; Col < Cols; ++Col) {
		for (int Row = 0; Row < Rows; ++Row) {
			AM3CellScheme* CellScheme = BoardScheme->Cells[Col + Row * Cols];
			if (!CellScheme) {
				CellScheme = World->SpawnActor<AM3CellScheme>(M3App->CellScheme_BP);
				CellScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
				CellScheme->Col = Col;
				CellScheme->Row = Row;
				BoardScheme->Cells[Col + Row * Cols] = CellScheme;
			}
			CellScheme->SetActorRelativeLocation(FVector(Row * 110, Col * 110, 0));
			CellScheme->Appointments.Empty();
		}
	}

	const auto& ElementIds = BoardScheme->ElementIds.Array();

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {
			const auto CellScheme = BoardScheme->Cells[i + j * Cols];

			EM3ElementId ElementId = EM3ElementId::UNKNOWN;
			do {
				int ElementIdIndex = FMath::RandRange(0, ElementIds.Num() - 1);
				ElementId = ElementIds[ElementIdIndex];
			} while ((i >= 2 &&
				BoardScheme->Cells[(i - 1) + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT) && BoardScheme->Cells[(i - 1) + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT)->Id == ElementId &&
				BoardScheme->Cells[(i - 2) + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT) && BoardScheme->Cells[(i - 2) + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT)->Id == ElementId)
				||
				(j >= 2 &&
					BoardScheme->Cells[i + (j - 1) * Cols]->GetAppointment(EM3CellAppointment::ELEMENT) && BoardScheme->Cells[i + (j - 1) * Cols]->GetAppointment(EM3CellAppointment::ELEMENT)->Id == ElementId &&
					BoardScheme->Cells[i + (j - 2) * Cols]->GetAppointment(EM3CellAppointment::ELEMENT) && BoardScheme->Cells[i + (j - 2) * Cols]->GetAppointment(EM3CellAppointment::ELEMENT)->Id == ElementId));

			UStaticMeshComponent* MeshComponent = nullptr;
			TArray<UActorComponent*> MeshesComponents = CellScheme->GetComponentsByClass(UStaticMeshComponent::StaticClass());
			for (UActorComponent* ActorComponent : MeshesComponents)
			{
				MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
				break;
			}

			switch (ElementId) {
				case EM3ElementId::ELEMENT_RED:
					CellScheme->AddAppointment(BoardScheme->ElementRedScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
					break;
				case EM3ElementId::ELEMENT_GREEN:
					CellScheme->AddAppointment(BoardScheme->ElementGreenScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
					break;
				case EM3ElementId::ELEMENT_BLUE:
					CellScheme->AddAppointment(BoardScheme->ElementBlueScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
					break;
				case EM3ElementId::ELEMENT_YELLOW:
					CellScheme->AddAppointment(BoardScheme->ElementYellowScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
					break;
				case EM3ElementId::ELEMENT_ORANGE:
					CellScheme->AddAppointment(BoardScheme->ElementOrangeScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
					break;
				case EM3ElementId::ELEMENT_PURPLE:
					CellScheme->AddAppointment(BoardScheme->ElementPurpleScheme);
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
					break;
				default:
					break;
			}
		}
	}
}

#endif
