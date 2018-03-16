// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardGeneratorComponent.h"
#include "M3App.h"
#include "M3Scheme.h"
#include "Engine/World.h"

UM3BoardGeneratorComponent::UM3BoardGeneratorComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3BoardGeneratorComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3BoardGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AM3BoardScheme* UM3BoardGeneratorComponent::Generate(AM3App* App) {

	const auto BoardScheme = GetWorld()->SpawnActor<AM3BoardScheme>(App->BoardScheme_BP);
	BoardScheme->AttachToActor(App, FAttachmentTransformRules::KeepWorldTransform);

	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	BoardScheme->Cells.SetNum(Cols * Rows);
	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {

			AM3CellScheme* CellScheme = GetWorld()->SpawnActor<AM3CellScheme>(App->CellScheme_BP);
			CellScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
			CellScheme->Col = i;
			CellScheme->Row = j;
			BoardScheme->Cells[i + j * Cols] = CellScheme;
		}
	}

	const auto ElementRedScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementRedScheme_BP);
	const auto ElementGreenScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementGreenScheme_BP);
	const auto ElementBlueScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementBlueScheme_BP);
	const auto ElementYellowScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementYellowScheme_BP);
	const auto ElementOrangeScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementOrangeScheme_BP);
	const auto ElementPurpleScheme = GetWorld()->SpawnActor<AM3CellAppointmentScheme>(App->ElementPurpleScheme_BP);

	ElementRedScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
	ElementGreenScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
	ElementBlueScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
	ElementYellowScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
	ElementOrangeScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);
	ElementPurpleScheme->AttachToActor(BoardScheme, FAttachmentTransformRules::KeepWorldTransform);

	const auto& ElementIds = BoardScheme->ElementIds.Array();

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {
			
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

			switch (ElementId) {
				case EM3ElementId::ELEMENT_RED: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementRedScheme);
				}
				break;
				case EM3ElementId::ELEMENT_GREEN: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementGreenScheme);
				}
				break;
				case EM3ElementId::ELEMENT_BLUE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementBlueScheme);
				}
				break;
				case EM3ElementId::ELEMENT_YELLOW: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementYellowScheme);
				}
				break;
				case EM3ElementId::ELEMENT_ORANGE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementOrangeScheme);
				}
				break;
				case EM3ElementId::ELEMENT_PURPLE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(ElementPurpleScheme);
				}
				break;
				default:
				break;
			}
		}
	}

	return BoardScheme;
}

