// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3EdModeProperties.h"
#include "M3Scheme.h"
#include "M3AssetsBundle.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITOR

#include "EngineUtils.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "EditorModeManager.h"

#endif

UM3EdModeProps_BoardCreate::UM3EdModeProps_BoardCreate() {
	static ConstructorHelpers::FClassFinder<AM3BoardScheme> BoardScheme_BP_RESOURCE(TEXT("Class'/M3/M3BoardScheme_BP.M3BoardScheme_BP_C'"));
	BoardScheme_BP = BoardScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellScheme> CellScheme_BP_RESOURCE(TEXT("Class'/M3/M3CellScheme_BP.M3CellScheme_BP_C'"));
	CellScheme_BP = CellScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentFunctionalCellClosedScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentFunctionalCellClosed_BP.M3AppointmentFunctionalCellClosed_BP_C'"));
	FunctionalCellClosedScheme_BP = AppointmentFunctionalCellClosedScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentFunctionalCellHoleScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentFunctionalCellHole_BP.M3AppointmentFunctionalCellHole_BP_C'"));
	FunctionalCellHoleScheme_BP = AppointmentFunctionalCellHoleScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentFunctionalCellRandomScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentFunctionalCellRandom_BP.M3AppointmentFunctionalCellRandom_BP_C'"));
	FunctionalCellRandomScheme_BP = AppointmentFunctionalCellRandomScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentFunctionalSpawnScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentFunctionalSpawn_BP.M3AppointmentFunctionalSpawn_BP_C'"));
	FunctionalSpawnScheme_BP = AppointmentFunctionalSpawnScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementRedScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementRed_BP.M3AppointmentElementRed_BP_C'"));
	ElementRedScheme_BP = AppointmentElementRedScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementGreenScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementGreen_BP.M3AppointmentElementGreen_BP_C'"));
	ElementGreenScheme_BP = AppointmentElementGreenScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementBlueScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementBlue_BP.M3AppointmentElementBlue_BP_C'"));
	ElementBlueScheme_BP = AppointmentElementBlueScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementYellowScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementYellow_BP.M3AppointmentElementYellow_BP_C'"));
	ElementYellowScheme_BP = AppointmentElementYellowScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementOrangeScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementOrange_BP.M3AppointmentElementOrange_BP_C'"));
	ElementOrangeScheme_BP = AppointmentElementOrangeScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementPurpleScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentElementPurple_BP.M3AppointmentElementPurple_BP_C'"));
	ElementPurpleScheme_BP = AppointmentElementPurpleScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentSuperElementMatch4Scheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentSuperElementMatch4_BP.M3AppointmentSuperElementMatch4_BP_C'"));
	SuperElementMatch4Scheme_BP = AppointmentSuperElementMatch4Scheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentSuperElementMatch5Scheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentSuperElementMatch5_BP.M3AppointmentSuperElementMatch5_BP_C'"));
	SuperElementMatch5Scheme_BP = AppointmentSuperElementMatch5Scheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentSuperElementMatch6Scheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentSuperElementMatch6_BP.M3AppointmentSuperElementMatch6_BP_C'"));
	SuperElementMatch6Scheme_BP = AppointmentSuperElementMatch6Scheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentSuperElementMatch7Scheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentSuperElementMatch7_BP.M3AppointmentSuperElementMatch7_BP_C'"));
	SuperElementMatch7Scheme_BP = AppointmentSuperElementMatch7Scheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerBox1XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerBox1X_BP.M3AppointmentBlockerBox1X_BP_C'"));
	BlockerBox1XScheme_BP = AppointmentBlockerBox1XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerBox2XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerBox2X_BP.M3AppointmentBlockerBox2X_BP_C'"));
	BlockerBox2XScheme_BP = AppointmentBlockerBox2XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerBox3XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerBox3X_BP.M3AppointmentBlockerBox3X_BP_C'"));
	BlockerBox3XScheme_BP = AppointmentBlockerBox3XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerIce1XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerIce1X_BP.M3AppointmentBlockerIce1X_BP_C'"));
	BlockerIce1XScheme_BP = AppointmentBlockerIce1XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerIce2XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerIce2X_BP.M3AppointmentBlockerIce2X_BP_C'"));
	BlockerIce2XScheme_BP = AppointmentBlockerIce2XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerWire1XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerWire1X_BP.M3AppointmentBlockerWire1X_BP_c'"));
	BlockerWire1XScheme_BP = AppointmentBlockerWire1XScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentBlockerWire2XScheme_BP_RESOURCE(TEXT("Class'/M3/M3AppointmentBlockerWire2X_BP.M3AppointmentBlockerWire2X_BP_c'"));
	BlockerWire2XScheme_BP = AppointmentBlockerWire2XScheme_BP_RESOURCE.Class;
}

#if WITH_EDITOR

void UM3EdModeProps_BoardReskin::PostEditChangeProperty(struct FPropertyChangedEvent& Event) {
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != nullptr) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UM3EdModeProps_BoardReskin, AssetsBundle_BP)) {
		if (AssetsBundle_BP) {
			UWorld* World = GEditor->GetEditorWorldContext().World();
			AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(this, AssetsBundle_BP));

			CellMaterial = AssetsBundle->Cell.Material;
			CellMesh = AssetsBundle->Cell.Mesh;

			RedElementMaterial = AssetsBundle->Element_RED.Material;
			RedElementMesh = AssetsBundle->Element_RED.Mesh;

			GreenElementMaterial = AssetsBundle->Element_GREEN.Material;
			GreenElementMesh = AssetsBundle->Element_GREEN.Mesh;

			BlueElementMaterial = AssetsBundle->Element_BLUE.Material;
			BlueElementMesh = AssetsBundle->Element_BLUE.Mesh;

			YellowElementMaterial = AssetsBundle->Element_YELLOW.Material;
			YellowElementMesh = AssetsBundle->Element_YELLOW.Mesh;

			OrangeElementMaterial = AssetsBundle->Element_ORANGE.Material;
			OrangeElementMesh = AssetsBundle->Element_ORANGE.Mesh;

			PurpleElementMaterial = AssetsBundle->Element_PURPLE.Material;
			PurpleElementMesh = AssetsBundle->Element_PURPLE.Mesh;

			SuperElementMatch4Material = AssetsBundle->SuperElement_MATCH4.Material;
			SuperElementMatch4Mesh = AssetsBundle->SuperElement_MATCH4.Mesh;

			SuperElementMatch5Material = AssetsBundle->SuperElement_MATCH5.Material;
			SuperElementMatch5Mesh = AssetsBundle->SuperElement_MATCH5.Mesh;

			SuperElementMatch6Material = AssetsBundle->SuperElement_MATCH6.Material;
			SuperElementMatch6Mesh = AssetsBundle->SuperElement_MATCH6.Mesh;

			SuperElementMatch7Material = AssetsBundle->SuperElement_MATCH7.Material;
			SuperElementMatch7Mesh = AssetsBundle->SuperElement_MATCH7.Mesh;

			BlockerBox1XMaterial = AssetsBundle->Blocker_Box1X.Material;
			BlockerBox1XMesh = AssetsBundle->Blocker_Box1X.Mesh;

			BlockerBox2XMaterial = AssetsBundle->Blocker_Box2X.Material;
			BlockerBox2XMesh = AssetsBundle->Blocker_Box2X.Mesh;

			BlockerBox3XMaterial = AssetsBundle->Blocker_Box3X.Material;
			BlockerBox3XMesh = AssetsBundle->Blocker_Box3X.Mesh;

			BlockerIce1XMaterial = AssetsBundle->Blocker_Ice1X.Material;
			BlockerIce1XMesh = AssetsBundle->Blocker_Ice1X.Mesh;

			BlockerIce2XMaterial = AssetsBundle->Blocker_Ice2X.Material;
			BlockerIce2XMesh = AssetsBundle->Blocker_Ice2X.Mesh;

			BlockerWire1XMaterial = AssetsBundle->Blocker_Wire1X.Material;
			BlockerWire1XMesh = AssetsBundle->Blocker_Wire1X.Mesh;

			BlockerWire2XMaterial = AssetsBundle->Blocker_Wire2X.Material;
			BlockerWire2XMesh = AssetsBundle->Blocker_Wire2X.Mesh;
		}
	}
}

#endif