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
				MAKE_BoardCreate_SLOT(this)
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
				MAKE_BoardReskin_SLOT(this)
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
		
			HexMapEdMode->EdModeProps_BoardScheme->Turns = M3App->BoardScheme->Turns;

			for (const auto Goal : M3App->BoardScheme->Goals) {
				switch (Goal->Id) {
					case EM3ElementId::ELEMENT_RED:
						HexMapEdMode->EdModeProps_BoardScheme->CollectRedElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->RedElementsQuantity = Goal->Quantity;
					break;
					case EM3ElementId::ELEMENT_GREEN:
						HexMapEdMode->EdModeProps_BoardScheme->CollectGreenElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->GreenElementsQuantity = Goal->Quantity;
					break;
					case EM3ElementId::ELEMENT_BLUE:
						HexMapEdMode->EdModeProps_BoardScheme->CollectBlueElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->BlueElementsQuantity = Goal->Quantity;
					break;
					case EM3ElementId::ELEMENT_YELLOW:
						HexMapEdMode->EdModeProps_BoardScheme->CollectYellowElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->YellowElementsQuantity = Goal->Quantity;
					break;
					case EM3ElementId::ELEMENT_ORANGE:
						HexMapEdMode->EdModeProps_BoardScheme->CollectOrangeElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->OrangeElementsQuantity = Goal->Quantity;
					break;
					case EM3ElementId::ELEMENT_PURPLE:
						HexMapEdMode->EdModeProps_BoardScheme->CollectPurpleElements = Goal->Enabled;
						HexMapEdMode->EdModeProps_BoardScheme->PurpleElementsQuantity = Goal->Quantity;
					break;
					default:
					break;
				}
			}
		}

		if (HexMapEdMode->EdModeProps_BoardReskin && M3App->AssetsBundle) {
			
			HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle_BP = M3App->AssetsBundle_BP;
			HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle = M3App->AssetsBundle;

			HexMapEdMode->EdModeProps_BoardReskin->RedElementMaterial = M3App->AssetsBundle->Element_RED.Material;
			HexMapEdMode->EdModeProps_BoardReskin->RedElementMesh = M3App->AssetsBundle->Element_RED.Mesh;

			HexMapEdMode->EdModeProps_BoardReskin->GreenElementMaterial = M3App->AssetsBundle->Element_GREEN.Material;
			HexMapEdMode->EdModeProps_BoardReskin->GreenElementMesh = M3App->AssetsBundle->Element_GREEN.Mesh;

			HexMapEdMode->EdModeProps_BoardReskin->BlueElementMaterial = M3App->AssetsBundle->Element_BLUE.Material;
			HexMapEdMode->EdModeProps_BoardReskin->BlueElementMesh = M3App->AssetsBundle->Element_BLUE.Mesh;

			HexMapEdMode->EdModeProps_BoardReskin->YellowElementMaterial = M3App->AssetsBundle->Element_YELLOW.Material;
			HexMapEdMode->EdModeProps_BoardReskin->YellowElementMesh = M3App->AssetsBundle->Element_YELLOW.Mesh;

			HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMaterial = M3App->AssetsBundle->Element_ORANGE.Material;
			HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMesh = M3App->AssetsBundle->Element_ORANGE.Mesh;

			HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMaterial = M3App->AssetsBundle->Element_PURPLE.Material;
			HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMesh = M3App->AssetsBundle->Element_PURPLE.Mesh;
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

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardCreate_SLOT(FHMEdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardCreate = PropEdModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanel_BoardCreate->SetObject(HexMapEdMode->EdModeProps_BoardCreate, true);
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
				SELF->EdModePanel_BoardCreate.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				MAKE_BoardCreate_BTN(LOCTEXT("M3BoardCreateBTN", "Create"))
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
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_BoardSchemeSave_BTN(LOCTEXT("M3BoardSchemeSaveBTN", "Save"))
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
						.IsChecked_Static(&FHMEdModeToolkit::ON_Element_CHECKED, static_cast<int>(EM3ElementId::UNKNOWN))
						.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
							if (State == ECheckBoxState::Checked) {
								SelectedElementId = EM3ElementId::UNKNOWN;
								AM3CellScheme::EdModeSelectedAppointmentScheme = nullptr;
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
								SNew(STextBlock)
								.ColorAndOpacity(FLinearColor::White)
								.Text(NSLOCTEXT("M3ElementNone", "M3ElementNone", "NONE"))
							]
						]
					]
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

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardReskin_SLOT(FHMEdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardReskin = PropEdModule.CreateDetailView(Args);
	FHMEdMode* HexMapEdMode = (FHMEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode) {
		SELF->EdModePanel_BoardReskin->SetObject(HexMapEdMode->EdModeProps_BoardReskin, true);
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
				.Text(NSLOCTEXT("M3BoardReskinHeader", "M3BoardReskinHeader", "Reskin Board"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanel_BoardReskin.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_BoardReskin_BTN(LOCTEXT("M3BoardReskinBTN", "Reskin"))
				]
			]
		];
}

FReply FHMEdModeToolkit::ON_BoardCreate_BTN() {
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	if (!HexMapEdMode->EdModeProps_BoardCreate->AssetsBundle_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Assets Bundle should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->BoardScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Board Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->CellScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Cell Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->FunctionalSpawnScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Spawn Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementRedScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Red Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementGreenScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Green Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementBlueScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Blue Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementYellowScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Yellow Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementOrangeScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Orange Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!HexMapEdMode->EdModeProps_BoardCreate->ElementPurpleScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Purple Element Scheme should be assigned!");
		return FReply::Handled();
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_CREATE", "M3_TRANSITION_BOARD_CREATE"));
	{
		AM3App* M3App = GetM3App();
		if (!M3App) {
			M3App = World->SpawnActor<AM3App>(FVector(0.f), FRotator(0.f));
		}

		M3App->AssetsBundle_BP = HexMapEdMode->EdModeProps_BoardCreate->AssetsBundle_BP;
		M3App->BoardScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->BoardScheme_BP;
		M3App->CellScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->CellScheme_BP;
		M3App->FunctionalSpawnScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->FunctionalSpawnScheme_BP;
		M3App->ElementRedScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementRedScheme_BP;
		M3App->ElementGreenScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementGreenScheme_BP;
		M3App->ElementBlueScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementBlueScheme_BP;
		M3App->ElementYellowScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementYellowScheme_BP;
		M3App->ElementOrangeScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementOrangeScheme_BP;
		M3App->ElementPurpleScheme_BP = HexMapEdMode->EdModeProps_BoardCreate->ElementPurpleScheme_BP;

		M3App->AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(M3App, M3App->AssetsBundle_BP));

		if (!M3App->BoardScheme) {
			M3App->BoardScheme = World->SpawnActor<AM3BoardScheme>(M3App->BoardScheme_BP);
			M3App->BoardScheme->AttachToActor(M3App, FAttachmentTransformRules::KeepWorldTransform);
		}
		M3App->BoardScheme->Cols = HexMapEdMode->EdModeProps_BoardCreate->Cols;
		M3App->BoardScheme->Rows = HexMapEdMode->EdModeProps_BoardCreate->Rows;

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

		GenerateElementsScheme(M3App);
		GenerateElementsVisual(M3App);
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_BoardReskin_BTN() {
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_RESKIN", "M3_TRANSITION_BOARD_RESKIN"));
	{
		AM3App* M3App = GetM3App();
		if (M3App) {
			if (HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle_BP && HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle) {
				M3App->AssetsBundle_BP = HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle_BP;
				M3App->AssetsBundle = HexMapEdMode->EdModeProps_BoardReskin->AssetsBundle;
			}

			if (HexMapEdMode->EdModeProps_BoardReskin->RedElementMaterial) {
				M3App->AssetsBundle->Element_RED.Material = HexMapEdMode->EdModeProps_BoardReskin->RedElementMaterial;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->GreenElementMaterial) {
				M3App->AssetsBundle->Element_GREEN.Material = HexMapEdMode->EdModeProps_BoardReskin->GreenElementMaterial;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->BlueElementMaterial) {
				M3App->AssetsBundle->Element_BLUE.Material = HexMapEdMode->EdModeProps_BoardReskin->BlueElementMaterial;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->YellowElementMaterial) {
				M3App->AssetsBundle->Element_YELLOW.Material = HexMapEdMode->EdModeProps_BoardReskin->YellowElementMaterial;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMaterial) {
				M3App->AssetsBundle->Element_ORANGE.Material = HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMaterial;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMaterial) {
				M3App->AssetsBundle->Element_PURPLE.Material = HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMaterial;
			}

			if (HexMapEdMode->EdModeProps_BoardReskin->RedElementMesh) {
				M3App->AssetsBundle->Element_RED.Mesh = HexMapEdMode->EdModeProps_BoardReskin->RedElementMesh;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->GreenElementMesh) {
				M3App->AssetsBundle->Element_GREEN.Mesh = HexMapEdMode->EdModeProps_BoardReskin->GreenElementMesh;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->BlueElementMesh) {
				M3App->AssetsBundle->Element_BLUE.Mesh = HexMapEdMode->EdModeProps_BoardReskin->BlueElementMesh;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->YellowElementMesh) {
				M3App->AssetsBundle->Element_YELLOW.Mesh = HexMapEdMode->EdModeProps_BoardReskin->YellowElementMesh;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMesh) {
				M3App->AssetsBundle->Element_ORANGE.Mesh = HexMapEdMode->EdModeProps_BoardReskin->OrangeElementMesh;
			}
			if (HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMesh) {
				M3App->AssetsBundle->Element_PURPLE.Mesh = HexMapEdMode->EdModeProps_BoardReskin->PurpleElementMesh;
			}
			FHMEdModeToolkit::GenerateElementsVisual(M3App);
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FHMEdModeToolkit::ON_BoardSchemeSave_BTN() {
	FHMEdMode* HexMapEdMode = (FHMEdMode*)(GLevelEditorModeTools().GetActiveMode(FHMEdMode::EM_HexMap));
	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_SCHEME_SAVE", "M3_TRANSITION_BOARD_SCHEME_SAVE"));
	{
		AM3App* M3App = GetM3App();
		if (M3App) {
			const auto BoardScheme = M3App->BoardScheme;

			BoardScheme->Turns = HexMapEdMode->EdModeProps_BoardScheme->Turns;

			BoardScheme->ElementIds.Empty();
			if (HexMapEdMode->EdModeProps_BoardScheme->RedElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_RED);
			}
			if (HexMapEdMode->EdModeProps_BoardScheme->GreenElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_GREEN);
			}
			if (HexMapEdMode->EdModeProps_BoardScheme->BlueElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_BLUE);
			}
			if (HexMapEdMode->EdModeProps_BoardScheme->YellowElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_YELLOW);
			}
			if (HexMapEdMode->EdModeProps_BoardScheme->OrangeElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_ORANGE);
			}
			if (HexMapEdMode->EdModeProps_BoardScheme->PurpleElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_PURPLE);
			}

			AM3GoalScheme* GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_RED);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectRedElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->RedElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_GREEN);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectGreenElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->GreenElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_BLUE);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectBlueElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->BlueElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_YELLOW);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectYellowElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->YellowElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_ORANGE);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectOrangeElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->OrangeElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3ElementId::ELEMENT_PURPLE);
			GoalScheme->Enabled = HexMapEdMode->EdModeProps_BoardScheme->CollectPurpleElements;
			GoalScheme->Quantity = HexMapEdMode->EdModeProps_BoardScheme->PurpleElementsQuantity;
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardCreate_BTN(const FText& Label) {
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_BoardCreate_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardReskin_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_BoardReskin_BTN);
}

TSharedRef<SWidget> FHMEdModeToolkit::MAKE_BoardSchemeSave_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHMEdModeToolkit::ON_BoardSchemeSave_BTN);
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

void FHMEdModeToolkit::GenerateElementsScheme(class AM3App* M3App) {

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
		}
	}
}

void FHMEdModeToolkit::GenerateElementsVisual(class AM3App* M3App) {
	const auto World = GEditor->GetEditorWorldContext().World();
	const auto BoardScheme = M3App->BoardScheme;
	const auto BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(M3App->AssetsBundle);

	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {
			const auto CellScheme = BoardScheme->Cells[i + j * Cols];

			UStaticMeshComponent* MeshComponent = nullptr;
			TArray<UActorComponent*> MeshesComponents = CellScheme->GetComponentsByClass(UStaticMeshComponent::StaticClass());
			for (UActorComponent* ActorComponent : MeshesComponents) {
				MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
				break;
			}

			switch (CellScheme->GetAppointment(EM3CellAppointment::ELEMENT)->Id) {
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
