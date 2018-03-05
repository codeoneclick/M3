// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardGeneratorComponent.h"
#include "M3App.h"
#include "M3Scheme.h"
#include "Engine/World.h"

UM3BoardGeneratorComponent::UM3BoardGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3BoardGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UM3BoardGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UM3BoardGeneratorComponent::Generate(AM3App* App) {
	AM3BoardScheme* BoardScheme = App->Scheme;
	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	BoardScheme->Cells.SetNum(Cols * Rows);
	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {
			AM3CellScheme* CellScheme = GetWorld()->SpawnActor<AM3CellScheme>(FVector(0, 0, 0), FRotator(0, 0, 0));
			CellScheme->Col = i;
			CellScheme->Row = j;
			BoardScheme->Cells[i + j * Cols] = CellScheme;
		}
	}

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
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementREDScheme);
				}
				break;
				case EM3ElementId::ELEMENT_GREEN: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementGREENScheme);
				}
				break;
				case EM3ElementId::ELEMENT_BLUE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementBLUEScheme);
				}
				break;
				case EM3ElementId::ELEMENT_YELLOW: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementYELLOWScheme);
				}
				break;
				case EM3ElementId::ELEMENT_ORANGE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementORANGEScheme);
				}
				break;
				case EM3ElementId::ELEMENT_PURPLE: {
					BoardScheme->Cells[i + j * Cols]->AddAppointment(App->CellElementPURPLEScheme);
				}
				break;
				default:
				break;
			}
		}
	}
}

