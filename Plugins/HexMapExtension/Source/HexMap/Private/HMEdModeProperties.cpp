// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMEdModeProperties.h"
#include "M3Scheme.h"
#include "UObject/ConstructorHelpers.h"

UM3EdModeProps_CreateBoard::UM3EdModeProps_CreateBoard() {
	static ConstructorHelpers::FClassFinder<AM3BoardScheme> BoardScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3BoardScheme_BP.M3BoardScheme_BP_C'"));
	BoardScheme_BP = BoardScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellScheme> CellScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3CellScheme_BP.M3CellScheme_BP_C'"));
	CellScheme_BP = CellScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentFunctionalSpawnScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentFunctionalSpawn_BP.M3AppointmentFunctionalSpawn_BP_C'"));
	FunctionalSpawnScheme_BP = AppointmentFunctionalSpawnScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementRedScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementRed_BP.M3AppointmentElementRed_BP_C'"));
	ElementRedScheme_BP = AppointmentElementRedScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementGreenScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementGreen_BP.M3AppointmentElementGreen_BP_C'"));
	ElementGreenScheme_BP = AppointmentElementGreenScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementBlueScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementBlue_BP.M3AppointmentElementBlue_BP_C'"));
	ElementBlueScheme_BP = AppointmentElementBlueScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementYellowScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementYellow_BP.M3AppointmentElementYellow_BP_C'"));
	ElementYellowScheme_BP = AppointmentElementYellowScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementOrangeScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementOrange_BP.M3AppointmentElementOrange_BP_C'"));
	ElementOrangeScheme_BP = AppointmentElementOrangeScheme_BP_RESOURCE.Class;

	static ConstructorHelpers::FClassFinder<AM3CellAppointmentScheme> AppointmentElementPurpleScheme_BP_RESOURCE(TEXT("Class'/HexMap/M3AppointmentElementPurple_BP.M3AppointmentElementPurple_BP_C'"));
	ElementPurpleScheme_BP = AppointmentElementPurpleScheme_BP_RESOURCE.Class;
}
