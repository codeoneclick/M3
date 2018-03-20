// Copyright serhii serhiiv 2017. All rights reserved.

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
}

void UM3EdModeProps_BoardReskin::PostEditChangeProperty(struct FPropertyChangedEvent& Event) {
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != nullptr) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UM3EdModeProps_BoardReskin, AssetsBundle_BP)) {
		if (AssetsBundle_BP) {
			UWorld* World = GEditor->GetEditorWorldContext().World();
			AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(this, AssetsBundle_BP));

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
		}
	}
}
