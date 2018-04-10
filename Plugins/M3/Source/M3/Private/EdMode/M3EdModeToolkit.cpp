// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3EdModeToolkit.h"

#if WITH_EDITOR

#include "M3EdMode.h"
#include "Engine.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SExpandableArea.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "M3EdModeProperties.h"
#include "EditorViewportClient.h"
#include "M3App.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"

EM3ElementId FM3EdModeToolkit::SelectedElementId = EM3ElementId::UNKNOWN;

#define LOCTEXT_NAMESPACE "HMEdModeToolkit"

void FM3EdModeToolkit::Init(const TSharedPtr<class IToolkitHost>& _ToolkitHost)
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
	FModeToolkit::Init(_ToolkitHost);
	FM3EdModeToolkit::FillBoardSchemeSettings();
}

FName FM3EdModeToolkit::GetToolkitFName() const
{
	return FName("M3EdModeToolkit");
}

FText FM3EdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("M3EdModeToolkit", "DisplayName", "M3 Tool");
}

class FEdMode* FM3EdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3);
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_Element_CheckBox_SLOT(FM3EdModeToolkit* SELF, EM3ElementId Id, AM3CellAppointmentScheme* Scheme, FLinearColor Color, FText Text) {
	return SNew(SCheckBox)
			.IsChecked_Static(&FM3EdModeToolkit::ON_Element_CHECKED, static_cast<int>(Id))
			.OnCheckStateChanged_Lambda([=](ECheckBoxState State) {
				if (State == ECheckBoxState::Checked) {
					SelectedElementId = Id;
					AM3CellScheme::EdModeSelectedAppointmentScheme = Scheme;
				}
			})
			.Content()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				.Padding(4.f)
				[
					SNew(SBox)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.WidthOverride(48)
					.HeightOverride(48)
					.MaxDesiredHeight(48)
					.MaxDesiredWidth(48)
					.Padding(2.f)
					[
						SNew(SImage)
						.ColorAndOpacity(Color)
					]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				.Padding(4.f)
				[
					SNew(SBox)
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					.WidthOverride(96)
					.HeightOverride(48)
					.Padding(2.f)
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.AutoWrapText(true)
						.Text(Text)
					]
				]
			];
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_Copyright_SLOT(FM3EdModeToolkit* SELF)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(32.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.AutoWrapText(true)
			.Text(LOCTEXT("CopyrightLabel", "Copyright (c) 2018 Serhii Serhiiv. All rights reserved."))
		];
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardCreate_SLOT(FM3EdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardCreate = PropEdModule.CreateDetailView(Args);
	FM3EdMode* EdMode = (FM3EdMode*)SELF->GetEditorMode();
	if (EdMode) {
		SELF->EdModePanel_BoardCreate->SetObject(EdMode->EdModeProps_BoardCreate, true);
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

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardScheme_SLOT(FM3EdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardScheme = PropEdModule.CreateDetailView(Args);
	FM3EdMode* EdMode = (FM3EdMode*)SELF->GetEditorMode();
	if (EdMode)
	{
		SELF->EdModePanel_BoardScheme->SetObject(EdMode->EdModeProps_BoardScheme, true);
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

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardElements_SLOT(FM3EdModeToolkit* SELF) {

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
					SNew(SUniformGridPanel)
					.SlotPadding(4.f)
					+ SUniformGridPanel::Slot(0, 0)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::UNKNOWN, nullptr, FLinearColor::White, NSLOCTEXT("M3ElementNone", "M3ElementNone", "NONE"))
					]
					+ SUniformGridPanel::Slot(1, 0)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::CELL_CLOSED, GetM3App()->BoardScheme->FunctionalCellClosedScheme, FLinearColor::Black, NSLOCTEXT("M3CellClosed", "M3CellClosed", "CLOSED"))
					]
					+ SUniformGridPanel::Slot(2, 0)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::CELL_HOLE, GetM3App()->BoardScheme->FunctionalCellHoleScheme, FLinearColor::Gray, NSLOCTEXT("M3CellHole", "M3CellHole", "HOLE"))
					]
					+ SUniformGridPanel::Slot(3, 0)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::CELL_RANDOM, GetM3App()->BoardScheme->FunctionalCellRandomScheme, FLinearColor::White, NSLOCTEXT("M3CellRandom", "M3CellRandom", "RANDOM"))
					]
					+ SUniformGridPanel::Slot(0, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_RED, GetM3App()->BoardScheme->ElementRedScheme, FLinearColor::Red, NSLOCTEXT("M3RedElement", "M3RedElement", "RED"))
					]
					+ SUniformGridPanel::Slot(1, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_GREEN, GetM3App()->BoardScheme->ElementGreenScheme, FLinearColor::Green, NSLOCTEXT("M3GreenElement", "M3GreenElement", "GREEN"))
					]
					+ SUniformGridPanel::Slot(2, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_BLUE, GetM3App()->BoardScheme->ElementBlueScheme, FLinearColor::Blue, NSLOCTEXT("M3BlueElement", "M3BlueElement", "BLUE"))
					]
					+ SUniformGridPanel::Slot(3, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_YELLOW, GetM3App()->BoardScheme->ElementYellowScheme, FLinearColor::Yellow, NSLOCTEXT("M3YellowElement", "M3YellowElement", "YELLOW"))
					]
					+ SUniformGridPanel::Slot(4, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_ORANGE, GetM3App()->BoardScheme->ElementOrangeScheme, FLinearColor(1.f, .5f, 0.f, 1.f), NSLOCTEXT("M3OrangeElement", "M3OrangeElement", "ORANGE"))
					]
					+ SUniformGridPanel::Slot(5, 1)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::ELEMENT_PURPLE, GetM3App()->BoardScheme->ElementPurpleScheme, FLinearColor(.5f, .0f, .5f, 1.f), NSLOCTEXT("M3PurpleElement", "M3PurpleElement", "PURPLE"))
					]
					+ SUniformGridPanel::Slot(0, 2)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::SUPERELEMENT_MATCH4, GetM3App()->BoardScheme->SuperElementMatch4Scheme, FLinearColor::Black, NSLOCTEXT("M3SuperElementMatch4", "M3SuperElementMatch4", "SUPER ELEMENT MATCH4"))
					]
					+ SUniformGridPanel::Slot(1, 2)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::SUPERELEMENT_MATCH5, GetM3App()->BoardScheme->SuperElementMatch5Scheme, FLinearColor::Black, NSLOCTEXT("M3SuperElementMatch5", "M3SuperElementMatch5", "SUPER ELEMENT MATCH5"))
					]
					+ SUniformGridPanel::Slot(2, 2)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::SUPERELEMENT_MATCH6, GetM3App()->BoardScheme->SuperElementMatch6Scheme, FLinearColor::Black, NSLOCTEXT("M3SuperElementMatch6", "M3SuperElementMatch6", "SUPER ELEMENT MATCH6"))
					]
					+ SUniformGridPanel::Slot(3, 2)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::SUPERELEMENT_MATCH7, GetM3App()->BoardScheme->SuperElementMatch7Scheme, FLinearColor::Black, NSLOCTEXT("M3SuperElementMatch7", "M3SuperElementMatch7", "SUPER ELEMENT MATCH7"))
					]
					+ SUniformGridPanel::Slot(0, 3)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_BOX1X, GetM3App()->BoardScheme->BlockerBox1XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerBox1X", "M3BlockerBox1X", "BLOCKER BOX1X"))
					]
					+ SUniformGridPanel::Slot(1, 3)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_BOX2X, GetM3App()->BoardScheme->BlockerBox2XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerBox2X", "M3BlockerBox2X", "BLOCKER BOX2X"))
					]
					+ SUniformGridPanel::Slot(2, 3)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_BOX3X, GetM3App()->BoardScheme->BlockerBox3XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerBox3X", "M3BlockerBox3X", "BLOCKER BOX3X"))
					]
					+ SUniformGridPanel::Slot(0, 4)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_ICE1X, GetM3App()->BoardScheme->BlockerIce1XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerIce1X", "M3BlockerIce1X", "BLOCKER ICE1X"))
					]
					+ SUniformGridPanel::Slot(1, 4)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_ICE2X, GetM3App()->BoardScheme->BlockerIce2XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerIce2X", "M3BlockerIce2X", "BLOCKER ICE2X"))
					]
					+ SUniformGridPanel::Slot(0, 5)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_WIRE1X, GetM3App()->BoardScheme->BlockerWire1XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerWire1X", "M3BlockerWire1X", "BLOCKER WIRE1X"))
					]
					+ SUniformGridPanel::Slot(1, 5)
					[
						MAKE_Element_CheckBox_SLOT(SELF, EM3ElementId::BLOCKER_WIRE2X, GetM3App()->BoardScheme->BlockerWire2XScheme, FLinearColor::Black, NSLOCTEXT("M3BlockerWire2X", "M3BlockerWire2X", "BLOCKER WIRE2X"))
					]
				]
		];
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardReskin_SLOT(FM3EdModeToolkit* SELF) {
	FPropertyEditorModule& PropEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanel_BoardReskin = PropEdModule.CreateDetailView(Args);
	FM3EdMode* EdMode = (FM3EdMode*)SELF->GetEditorMode();
	if (EdMode) {
		SELF->EdModePanel_BoardReskin->SetObject(EdMode->EdModeProps_BoardReskin, true);
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

FReply FM3EdModeToolkit::ON_BoardCreate_BTN() {
	FM3EdMode* EdMode = (FM3EdMode*)(GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3));
	if (!EdMode->EdModeProps_BoardCreate->AssetsBundle_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Assets Bundle should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->BoardScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Board Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->CellScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Cell Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->FunctionalCellClosedScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Closed Cell should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->FunctionalCellHoleScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Cell Hole should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->FunctionalCellRandomScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Cell Random should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->FunctionalSpawnScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Functional Spawn Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementRedScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Red Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementGreenScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Green Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementBlueScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Blue Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementYellowScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Yellow Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementOrangeScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Orange Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->ElementPurpleScheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Purple Element Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->SuperElementMatch4Scheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Super Element Match 4 Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->SuperElementMatch5Scheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Super Element Match 5 Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->SuperElementMatch6Scheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Super Element Match 6 Scheme should be assigned!");
		return FReply::Handled();
	}
	if (!EdMode->EdModeProps_BoardCreate->SuperElementMatch7Scheme_BP) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Super Element Match 7 Scheme should be assigned!");
		return FReply::Handled();
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_CREATE", "M3_TRANSITION_BOARD_CREATE"));
	{
		AM3App* M3App = GetM3App();
		if (!M3App) {
			M3App = World->SpawnActor<AM3App>(FVector(0.f), FRotator(0.f));
		}

		M3App->AssetsBundle_BP = EdMode->EdModeProps_BoardCreate->AssetsBundle_BP;
		M3App->BoardScheme_BP = EdMode->EdModeProps_BoardCreate->BoardScheme_BP;

		M3App->AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(M3App, M3App->AssetsBundle_BP));

		if (!M3App->BoardScheme) {
			M3App->BoardScheme = World->SpawnActor<AM3BoardScheme>(M3App->BoardScheme_BP);
			M3App->BoardScheme->AttachToActor(M3App, FAttachmentTransformRules::KeepWorldTransform);
		}

		M3App->BoardScheme->Cols = EdMode->EdModeProps_BoardCreate->Cols;
		M3App->BoardScheme->Rows = EdMode->EdModeProps_BoardCreate->Rows;
		M3App->BoardScheme->ElementSize = EdMode->EdModeProps_BoardCreate->ElementSize;

		float LocationX = -M3App->BoardScheme->Rows * M3App->BoardScheme->ElementSize.X * 0.5 + M3App->BoardScheme->ElementSize.X * 0.5;
		float LocationY = -M3App->BoardScheme->Cols * M3App->BoardScheme->ElementSize.Y * 0.5 + M3App->BoardScheme->ElementSize.Y * 0.5;
		FVector CurrentLocation = M3App->BoardScheme->GetActorLocation();
		M3App->BoardScheme->SetActorLocation(FVector(LocationX, LocationY, CurrentLocation.Z));

		if (!M3App->BoardScheme->FunctionalCellClosedScheme) {
			M3App->BoardScheme->FunctionalCellClosedScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->FunctionalCellClosedScheme_BP);
			M3App->BoardScheme->FunctionalCellClosedScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->FunctionalCellHoleScheme) {
			M3App->BoardScheme->FunctionalCellHoleScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->FunctionalCellHoleScheme_BP);
			M3App->BoardScheme->FunctionalCellHoleScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->FunctionalCellRandomScheme) {
			M3App->BoardScheme->FunctionalCellRandomScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->FunctionalCellRandomScheme_BP);
			M3App->BoardScheme->FunctionalCellRandomScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementRedScheme) {
			M3App->BoardScheme->ElementRedScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementRedScheme_BP);
			M3App->BoardScheme->ElementRedScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementGreenScheme) {
			M3App->BoardScheme->ElementGreenScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementGreenScheme_BP);
			M3App->BoardScheme->ElementGreenScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementBlueScheme) {
			M3App->BoardScheme->ElementBlueScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementBlueScheme_BP);
			M3App->BoardScheme->ElementBlueScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementYellowScheme) {
			M3App->BoardScheme->ElementYellowScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementYellowScheme_BP);
			M3App->BoardScheme->ElementYellowScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementOrangeScheme) {
			M3App->BoardScheme->ElementOrangeScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementOrangeScheme_BP);
			M3App->BoardScheme->ElementOrangeScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->ElementPurpleScheme) {
			M3App->BoardScheme->ElementPurpleScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->ElementPurpleScheme_BP);
			M3App->BoardScheme->ElementPurpleScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->SuperElementMatch4Scheme) {
			M3App->BoardScheme->SuperElementMatch4Scheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->SuperElementMatch4Scheme_BP);
			M3App->BoardScheme->SuperElementMatch4Scheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->SuperElementMatch5Scheme) {
			M3App->BoardScheme->SuperElementMatch5Scheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->SuperElementMatch5Scheme_BP);
			M3App->BoardScheme->SuperElementMatch5Scheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->SuperElementMatch6Scheme) {
			M3App->BoardScheme->SuperElementMatch6Scheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->SuperElementMatch6Scheme_BP);
			M3App->BoardScheme->SuperElementMatch6Scheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->SuperElementMatch7Scheme) {
			M3App->BoardScheme->SuperElementMatch7Scheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->SuperElementMatch7Scheme_BP);
			M3App->BoardScheme->SuperElementMatch7Scheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerBox1XScheme) {
			M3App->BoardScheme->BlockerBox1XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerBox1XScheme_BP);
			M3App->BoardScheme->BlockerBox1XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerBox2XScheme) {
			M3App->BoardScheme->BlockerBox2XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerBox2XScheme_BP);
			M3App->BoardScheme->BlockerBox2XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerBox3XScheme) {
			M3App->BoardScheme->BlockerBox3XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerBox3XScheme_BP);
			M3App->BoardScheme->BlockerBox3XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerIce1XScheme) {
			M3App->BoardScheme->BlockerIce1XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerIce1XScheme_BP);
			M3App->BoardScheme->BlockerIce1XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerIce2XScheme) {
			M3App->BoardScheme->BlockerIce2XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerIce2XScheme_BP);
			M3App->BoardScheme->BlockerIce2XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerWire1XScheme) {
			M3App->BoardScheme->BlockerWire1XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerWire1XScheme_BP);
			M3App->BoardScheme->BlockerWire1XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		if (!M3App->BoardScheme->BlockerWire2XScheme) {
			M3App->BoardScheme->BlockerWire2XScheme = World->SpawnActor<AM3CellAppointmentScheme>(EdMode->EdModeProps_BoardCreate->BlockerWire2XScheme_BP);
			M3App->BoardScheme->BlockerWire2XScheme->AttachToActor(M3App->BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
		}

		GenerateElementsScheme(M3App);
		GenerateElementsVisual(M3App);
	}
	GEditor->EndTransaction();

	FM3EdModeToolkit::FillBoardSchemeSettings();

	return FReply::Handled();
}

FReply FM3EdModeToolkit::ON_BoardReskin_BTN() {
	FM3EdMode* EdMode = (FM3EdMode*)(GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3));
	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_RESKIN", "M3_TRANSITION_BOARD_RESKIN"));
	{
		AM3App* M3App = GetM3App();
		if (M3App) {
			if (EdMode->EdModeProps_BoardReskin->AssetsBundle_BP && EdMode->EdModeProps_BoardReskin->AssetsBundle) {
				M3App->AssetsBundle_BP = EdMode->EdModeProps_BoardReskin->AssetsBundle_BP;
				M3App->AssetsBundle = EdMode->EdModeProps_BoardReskin->AssetsBundle;
			}

			M3App->BoardScheme->ElementSize = EdMode->EdModeProps_BoardReskin->ElementSize;

			float LocationX = -M3App->BoardScheme->Rows * M3App->BoardScheme->ElementSize.X * 0.5 + M3App->BoardScheme->ElementSize.X * 0.5;
			float LocationY = -M3App->BoardScheme->Cols * M3App->BoardScheme->ElementSize.Y * 0.5 + M3App->BoardScheme->ElementSize.Y * 0.5;
			FVector CurrentLocation = M3App->BoardScheme->GetActorLocation();
			M3App->BoardScheme->SetActorLocation(FVector(LocationX, LocationY, CurrentLocation.Z));

			for (int Col = 0; Col < M3App->BoardScheme->Cols; ++Col) {
				for (int Row = 0; Row < M3App->BoardScheme->Rows; ++Row) {
					AM3CellScheme* CellScheme = M3App->BoardScheme->Cells[Col + Row * M3App->BoardScheme->Cols];
					if (CellScheme) {
						CellScheme->SetActorRelativeLocation(FVector(Row * M3App->BoardScheme->ElementSize.X, Col * M3App->BoardScheme->ElementSize.Y, 0));
					}
				}
			}

			if (EdMode->EdModeProps_BoardReskin->CellMaterial) {
				M3App->AssetsBundle->Cell.Material = EdMode->EdModeProps_BoardReskin->CellMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->RedElementMaterial) {
				M3App->AssetsBundle->Element_RED.Material = EdMode->EdModeProps_BoardReskin->RedElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->GreenElementMaterial) {
				M3App->AssetsBundle->Element_GREEN.Material = EdMode->EdModeProps_BoardReskin->GreenElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlueElementMaterial) {
				M3App->AssetsBundle->Element_BLUE.Material = EdMode->EdModeProps_BoardReskin->BlueElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->YellowElementMaterial) {
				M3App->AssetsBundle->Element_YELLOW.Material = EdMode->EdModeProps_BoardReskin->YellowElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->OrangeElementMaterial) {
				M3App->AssetsBundle->Element_ORANGE.Material = EdMode->EdModeProps_BoardReskin->OrangeElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->PurpleElementMaterial) {
				M3App->AssetsBundle->Element_PURPLE.Material = EdMode->EdModeProps_BoardReskin->PurpleElementMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch4Material) {
				M3App->AssetsBundle->SuperElement_MATCH4.Material = EdMode->EdModeProps_BoardReskin->SuperElementMatch4Material;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch5Material) {
				M3App->AssetsBundle->SuperElement_MATCH5.Material = EdMode->EdModeProps_BoardReskin->SuperElementMatch5Material;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch6Material) {
				M3App->AssetsBundle->SuperElement_MATCH6.Material = EdMode->EdModeProps_BoardReskin->SuperElementMatch6Material;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch7Material) {
				M3App->AssetsBundle->SuperElement_MATCH7.Material = EdMode->EdModeProps_BoardReskin->SuperElementMatch7Material;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox1XMaterial) {
				M3App->AssetsBundle->Blocker_Box1X.Material = EdMode->EdModeProps_BoardReskin->BlockerBox1XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox2XMaterial) {
				M3App->AssetsBundle->Blocker_Box2X.Material = EdMode->EdModeProps_BoardReskin->BlockerBox2XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox3XMaterial) {
				M3App->AssetsBundle->Blocker_Box3X.Material = EdMode->EdModeProps_BoardReskin->BlockerBox3XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerIce1XMaterial) {
				M3App->AssetsBundle->Blocker_Ice1X.Material = EdMode->EdModeProps_BoardReskin->BlockerIce1XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerIce2XMaterial) {
				M3App->AssetsBundle->Blocker_Ice2X.Material = EdMode->EdModeProps_BoardReskin->BlockerIce2XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerWire1XMaterial) {
				M3App->AssetsBundle->Blocker_Wire1X.Material = EdMode->EdModeProps_BoardReskin->BlockerWire1XMaterial;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerWire2XMaterial) {
				M3App->AssetsBundle->Blocker_Wire2X.Material = EdMode->EdModeProps_BoardReskin->BlockerWire2XMaterial;
			}

			if (EdMode->EdModeProps_BoardReskin->CellMesh) {
				M3App->AssetsBundle->Cell.Mesh = EdMode->EdModeProps_BoardReskin->CellMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->RedElementMesh) {
				M3App->AssetsBundle->Element_RED.Mesh = EdMode->EdModeProps_BoardReskin->RedElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->GreenElementMesh) {
				M3App->AssetsBundle->Element_GREEN.Mesh = EdMode->EdModeProps_BoardReskin->GreenElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlueElementMesh) {
				M3App->AssetsBundle->Element_BLUE.Mesh = EdMode->EdModeProps_BoardReskin->BlueElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->YellowElementMesh) {
				M3App->AssetsBundle->Element_YELLOW.Mesh = EdMode->EdModeProps_BoardReskin->YellowElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->OrangeElementMesh) {
				M3App->AssetsBundle->Element_ORANGE.Mesh = EdMode->EdModeProps_BoardReskin->OrangeElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->PurpleElementMesh) {
				M3App->AssetsBundle->Element_PURPLE.Mesh = EdMode->EdModeProps_BoardReskin->PurpleElementMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch4Mesh) {
				M3App->AssetsBundle->SuperElement_MATCH4.Mesh = EdMode->EdModeProps_BoardReskin->SuperElementMatch4Mesh;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch5Mesh) {
				M3App->AssetsBundle->SuperElement_MATCH5.Mesh = EdMode->EdModeProps_BoardReskin->SuperElementMatch5Mesh;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch6Mesh) {
				M3App->AssetsBundle->SuperElement_MATCH6.Mesh = EdMode->EdModeProps_BoardReskin->SuperElementMatch6Mesh;
			}
			if (EdMode->EdModeProps_BoardReskin->SuperElementMatch7Mesh) {
				M3App->AssetsBundle->SuperElement_MATCH7.Mesh = EdMode->EdModeProps_BoardReskin->SuperElementMatch7Mesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox1XMesh) {
				M3App->AssetsBundle->Blocker_Box1X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerBox1XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox2XMesh) {
				M3App->AssetsBundle->Blocker_Box2X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerBox2XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerBox3XMesh) {
				M3App->AssetsBundle->Blocker_Box3X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerBox3XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerIce1XMesh) {
				M3App->AssetsBundle->Blocker_Ice1X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerIce1XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerIce2XMesh) {
				M3App->AssetsBundle->Blocker_Ice2X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerIce2XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerWire1XMesh) {
				M3App->AssetsBundle->Blocker_Wire1X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerWire1XMesh;
			}
			if (EdMode->EdModeProps_BoardReskin->BlockerWire2XMesh) {
				M3App->AssetsBundle->Blocker_Wire2X.Mesh = EdMode->EdModeProps_BoardReskin->BlockerWire2XMesh;
			}

			GenerateElementsVisual(M3App);
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FM3EdModeToolkit::ON_BoardSchemeSave_BTN() {
	FM3EdMode* EdMode = (FM3EdMode*)(GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3));
	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("M3_TRANSITION_BOARD_SCHEME_SAVE", "M3_TRANSITION_BOARD_SCHEME_SAVE"));
	{
		AM3App* M3App = GetM3App();
		if (M3App) {
			const auto BoardScheme = M3App->BoardScheme;

			BoardScheme->IsTurnBased = EdMode->EdModeProps_BoardScheme->IsTurnBased;
			BoardScheme->Duration = EdMode->EdModeProps_BoardScheme->Duration;
			BoardScheme->OneMatchScores = EdMode->EdModeProps_BoardScheme->OneMatchScores;
			BoardScheme->IsUseComboMatchScores = EdMode->EdModeProps_BoardScheme->IsUseComboMatchScores;
			BoardScheme->ComboMatchScoresMultiplier = EdMode->EdModeProps_BoardScheme->ComboMatchScoresMultiplier;
			BoardScheme->Star1Scores = EdMode->EdModeProps_BoardScheme->Star1Scores;
			BoardScheme->Star2Scores = EdMode->EdModeProps_BoardScheme->Star2Scores;
			BoardScheme->Star3Scores = EdMode->EdModeProps_BoardScheme->Star3Scores;

			BoardScheme->ElementIds.Empty();
			if (EdMode->EdModeProps_BoardScheme->RedElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_RED);
			}
			if (EdMode->EdModeProps_BoardScheme->GreenElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_GREEN);
			}
			if (EdMode->EdModeProps_BoardScheme->BlueElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_BLUE);
			}
			if (EdMode->EdModeProps_BoardScheme->YellowElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_YELLOW);
			}
			if (EdMode->EdModeProps_BoardScheme->OrangeElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_ORANGE);
			}
			if (EdMode->EdModeProps_BoardScheme->PurpleElementExist) {
				BoardScheme->ElementIds.Add(EM3ElementId::ELEMENT_PURPLE);
			}

			AM3GoalScheme* GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_RED_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectRedElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->RedElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_GREEN_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectGreenElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->GreenElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_BLUE_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectBlueElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->BlueElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_YELLOW_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectYellowElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->YellowElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_ORANGE_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectOrangeElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->OrangeElementsQuantity;

			GoalScheme = BoardScheme->GetGoalScheme(World, EM3GoalId::COLLECT_PURPLE_ELEMENTS);
			GoalScheme->Enabled = EdMode->EdModeProps_BoardScheme->CollectPurpleElements;
			GoalScheme->Quantity = EdMode->EdModeProps_BoardScheme->PurpleElementsQuantity;
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardCreate_BTN(const FText& Label) {
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FM3EdModeToolkit::ON_BoardCreate_BTN);
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardReskin_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FM3EdModeToolkit::ON_BoardReskin_BTN);
}

TSharedRef<SWidget> FM3EdModeToolkit::MAKE_BoardSchemeSave_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FM3EdModeToolkit::ON_BoardSchemeSave_BTN);
}

ECheckBoxState FM3EdModeToolkit::ON_Element_CHECKED(int ElementId) {
	return ElementId == static_cast<int>(SelectedElementId) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

AM3App* FM3EdModeToolkit::GetM3App() {
	UWorld* World = GEditor->GetEditorWorldContext().World();
	AM3App* M3App = nullptr;
	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		M3App = static_cast<AM3App*>(*It);
		break;
	}
	return M3App;
}

void FM3EdModeToolkit::GenerateElementsScheme(class AM3App* M3App) {

	FM3EdMode* EdMode = (FM3EdMode*)(GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3));
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
				CellScheme = World->SpawnActor<AM3CellScheme>(EdMode->EdModeProps_BoardCreate->CellScheme_BP);
				CellScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
				CellScheme->Col = Col;
				CellScheme->Row = Row;
				BoardScheme->Cells[Col + Row * Cols] = CellScheme;
			}
			CellScheme->SetActorRelativeLocation(FVector(Row * BoardScheme->ElementSize.X, Col * BoardScheme->ElementSize.Y, 0));
			CellScheme->RemoveAllAppointments();
		}
	}

	const auto& ElementIds = BoardScheme->ElementIds.Array();
	if (ElementIds.Num() == 0) {
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Elements Id's should exist!");
	} else {
		for (int i = 0; i < Cols; ++i) {
			for (int j = 0; j < Rows; ++j) {
				const auto CellScheme = BoardScheme->Cells[i + j * Cols];

				EM3ElementId ElementId = EM3ElementId::UNKNOWN;
				do {
					int ElementIdIndex = FMath::RandRange(0, ElementIds.Num() - 1);
					ElementId = ElementIds[ElementIdIndex];
				} while ((i >= 2 &&
					BoardScheme->Cells[(i - 1) + j * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT) && BoardScheme->Cells[(i - 1) + j * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT)->Id == ElementId &&
					BoardScheme->Cells[(i - 2) + j * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT) && BoardScheme->Cells[(i - 2) + j * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT)->Id == ElementId)
					||
					(j >= 2 &&
						BoardScheme->Cells[i + (j - 1) * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT) && BoardScheme->Cells[i + (j - 1) * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT)->Id == ElementId &&
						BoardScheme->Cells[i + (j - 2) * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT) && BoardScheme->Cells[i + (j - 2) * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT)->Id == ElementId));
				
				switch (ElementId) {
					case EM3ElementId::ELEMENT_RED:
						CellScheme->AddAppointment(BoardScheme->ElementRedScheme);
					break;
					case EM3ElementId::ELEMENT_GREEN:
						CellScheme->AddAppointment(BoardScheme->ElementGreenScheme);
					break;
					case EM3ElementId::ELEMENT_BLUE:
						CellScheme->AddAppointment(BoardScheme->ElementBlueScheme);
					break;
					case EM3ElementId::ELEMENT_YELLOW:
						CellScheme->AddAppointment(BoardScheme->ElementYellowScheme);
					break;
					case EM3ElementId::ELEMENT_ORANGE:
						CellScheme->AddAppointment(BoardScheme->ElementOrangeScheme);
					break;
					case EM3ElementId::ELEMENT_PURPLE:
						CellScheme->AddAppointment(BoardScheme->ElementPurpleScheme);
					break;
				default:
					break;
				}
			}
		}
	}
}

void FM3EdModeToolkit::GenerateElementsVisual(class AM3App* M3App) {
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

			if (CellScheme->IsAppointmentExist(EM3CellAppointment::REGULARELEMENT)) {
				switch (CellScheme->GetAppointment(EM3CellAppointment::REGULARELEMENT)->Id) {
				case EM3ElementId::ELEMENT_RED:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
					break;
				case EM3ElementId::ELEMENT_GREEN:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
					break;
				case EM3ElementId::ELEMENT_BLUE:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
					break;
				case EM3ElementId::ELEMENT_YELLOW:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
					break;
				case EM3ElementId::ELEMENT_ORANGE:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
					break;
				case EM3ElementId::ELEMENT_PURPLE:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
					break;
				default:
					break;
				}
			}

			if (CellScheme->IsAppointmentExist(EM3CellAppointment::SUPERELEMENT)) {
				switch (CellScheme->GetAppointment(EM3CellAppointment::SUPERELEMENT)->Id) {
				case EM3ElementId::SUPERELEMENT_MATCH4:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH4.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH4.Material);
					break;
				case EM3ElementId::SUPERELEMENT_MATCH5:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH5.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH5.Material);
					break;
				case EM3ElementId::SUPERELEMENT_MATCH6:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH6.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH6.Material);
					break;
				case EM3ElementId::SUPERELEMENT_MATCH7:
					MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH7.Mesh);
					MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH7.Material);
					break;
				default:
					break;
				}
			}
		}
	}
}

void FM3EdModeToolkit::FillBoardSchemeSettings() {
	UWorld* World = GEditor->GetEditorWorldContext().World();
	AM3App* M3App = nullptr;

	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		M3App = static_cast<AM3App*>(*It);
		break;
	}

	if (M3App) {
		FM3EdMode* EdMode = (FM3EdMode*)(GLevelEditorModeTools().GetActiveMode(FM3EdMode::EM_M3));
		if (EdMode->EdModeProps_BoardScheme && M3App->BoardScheme) {
			EdMode->EdModeProps_BoardScheme->RedElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_RED);
			EdMode->EdModeProps_BoardScheme->GreenElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_GREEN);
			EdMode->EdModeProps_BoardScheme->BlueElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_BLUE);
			EdMode->EdModeProps_BoardScheme->YellowElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_YELLOW);
			EdMode->EdModeProps_BoardScheme->OrangeElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_ORANGE);
			EdMode->EdModeProps_BoardScheme->PurpleElementExist = M3App->BoardScheme->ElementIds.Contains(EM3ElementId::ELEMENT_PURPLE);

			EdMode->EdModeProps_BoardScheme->IsTurnBased = M3App->BoardScheme->IsTurnBased;
			EdMode->EdModeProps_BoardScheme->Duration = M3App->BoardScheme->Duration;
			EdMode->EdModeProps_BoardScheme->OneMatchScores = M3App->BoardScheme->OneMatchScores;
			EdMode->EdModeProps_BoardScheme->IsUseComboMatchScores = M3App->BoardScheme->IsUseComboMatchScores;
			EdMode->EdModeProps_BoardScheme->ComboMatchScoresMultiplier = M3App->BoardScheme->ComboMatchScoresMultiplier;
			EdMode->EdModeProps_BoardScheme->Star1Scores = M3App->BoardScheme->Star1Scores;
			EdMode->EdModeProps_BoardScheme->Star2Scores = M3App->BoardScheme->Star2Scores;
			EdMode->EdModeProps_BoardScheme->Star3Scores = M3App->BoardScheme->Star3Scores;

			for (const auto Goal : M3App->BoardScheme->Goals) {
				switch (Goal->Id) {
				case EM3GoalId::COLLECT_RED_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectRedElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->RedElementsQuantity = Goal->Quantity;
					break;
				case EM3GoalId::COLLECT_GREEN_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectGreenElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->GreenElementsQuantity = Goal->Quantity;
					break;
				case EM3GoalId::COLLECT_BLUE_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectBlueElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->BlueElementsQuantity = Goal->Quantity;
					break;
				case EM3GoalId::COLLECT_YELLOW_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectYellowElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->YellowElementsQuantity = Goal->Quantity;
					break;
				case EM3GoalId::COLLECT_ORANGE_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectOrangeElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->OrangeElementsQuantity = Goal->Quantity;
					break;
				case EM3GoalId::COLLECT_PURPLE_ELEMENTS:
					EdMode->EdModeProps_BoardScheme->CollectPurpleElements = Goal->Enabled;
					EdMode->EdModeProps_BoardScheme->PurpleElementsQuantity = Goal->Quantity;
					break;
				default:
					break;
				}
			}
		}

		if (EdMode->EdModeProps_BoardReskin && M3App->AssetsBundle) {

			EdMode->EdModeProps_BoardReskin->AssetsBundle_BP = M3App->AssetsBundle_BP;
			EdMode->EdModeProps_BoardReskin->AssetsBundle = M3App->AssetsBundle;

			EdMode->EdModeProps_BoardReskin->ElementSize = M3App->BoardScheme->ElementSize;

			EdMode->EdModeProps_BoardReskin->CellMaterial = M3App->AssetsBundle->Cell.Material;
			EdMode->EdModeProps_BoardReskin->CellMesh = M3App->AssetsBundle->Cell.Mesh;

			EdMode->EdModeProps_BoardReskin->RedElementMaterial = M3App->AssetsBundle->Element_RED.Material;
			EdMode->EdModeProps_BoardReskin->RedElementMesh = M3App->AssetsBundle->Element_RED.Mesh;

			EdMode->EdModeProps_BoardReskin->GreenElementMaterial = M3App->AssetsBundle->Element_GREEN.Material;
			EdMode->EdModeProps_BoardReskin->GreenElementMesh = M3App->AssetsBundle->Element_GREEN.Mesh;

			EdMode->EdModeProps_BoardReskin->BlueElementMaterial = M3App->AssetsBundle->Element_BLUE.Material;
			EdMode->EdModeProps_BoardReskin->BlueElementMesh = M3App->AssetsBundle->Element_BLUE.Mesh;

			EdMode->EdModeProps_BoardReskin->YellowElementMaterial = M3App->AssetsBundle->Element_YELLOW.Material;
			EdMode->EdModeProps_BoardReskin->YellowElementMesh = M3App->AssetsBundle->Element_YELLOW.Mesh;

			EdMode->EdModeProps_BoardReskin->OrangeElementMaterial = M3App->AssetsBundle->Element_ORANGE.Material;
			EdMode->EdModeProps_BoardReskin->OrangeElementMesh = M3App->AssetsBundle->Element_ORANGE.Mesh;

			EdMode->EdModeProps_BoardReskin->PurpleElementMaterial = M3App->AssetsBundle->Element_PURPLE.Material;
			EdMode->EdModeProps_BoardReskin->PurpleElementMesh = M3App->AssetsBundle->Element_PURPLE.Mesh;

			EdMode->EdModeProps_BoardReskin->SuperElementMatch4Material = M3App->AssetsBundle->SuperElement_MATCH4.Material;
			EdMode->EdModeProps_BoardReskin->SuperElementMatch4Mesh = M3App->AssetsBundle->SuperElement_MATCH4.Mesh;

			EdMode->EdModeProps_BoardReskin->SuperElementMatch5Material = M3App->AssetsBundle->SuperElement_MATCH5.Material;
			EdMode->EdModeProps_BoardReskin->SuperElementMatch5Mesh = M3App->AssetsBundle->SuperElement_MATCH5.Mesh;

			EdMode->EdModeProps_BoardReskin->SuperElementMatch6Material = M3App->AssetsBundle->SuperElement_MATCH6.Material;
			EdMode->EdModeProps_BoardReskin->SuperElementMatch6Mesh = M3App->AssetsBundle->SuperElement_MATCH6.Mesh;

			EdMode->EdModeProps_BoardReskin->SuperElementMatch7Material = M3App->AssetsBundle->SuperElement_MATCH7.Material;
			EdMode->EdModeProps_BoardReskin->SuperElementMatch7Mesh = M3App->AssetsBundle->SuperElement_MATCH7.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerBox1XMaterial = M3App->AssetsBundle->Blocker_Box1X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerBox1XMesh = M3App->AssetsBundle->Blocker_Box1X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerBox2XMaterial = M3App->AssetsBundle->Blocker_Box2X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerBox2XMesh = M3App->AssetsBundle->Blocker_Box2X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerBox3XMaterial = M3App->AssetsBundle->Blocker_Box3X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerBox3XMesh = M3App->AssetsBundle->Blocker_Box3X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerIce1XMaterial = M3App->AssetsBundle->Blocker_Ice1X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerIce1XMesh = M3App->AssetsBundle->Blocker_Ice1X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerIce2XMaterial = M3App->AssetsBundle->Blocker_Ice2X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerIce2XMesh = M3App->AssetsBundle->Blocker_Ice2X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerWire1XMaterial = M3App->AssetsBundle->Blocker_Wire1X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerWire1XMesh = M3App->AssetsBundle->Blocker_Wire1X.Mesh;

			EdMode->EdModeProps_BoardReskin->BlockerWire2XMaterial = M3App->AssetsBundle->Blocker_Wire2X.Material;
			EdMode->EdModeProps_BoardReskin->BlockerWire2XMesh = M3App->AssetsBundle->Blocker_Wire2X.Mesh;
		}
	}
}

#endif
