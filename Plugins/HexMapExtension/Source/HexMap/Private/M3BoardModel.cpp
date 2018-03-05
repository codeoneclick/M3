// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3Scheme.h"

FORWARD_DECL_STRONG(M3CellModel)
FORWARD_DECL_STRONG(M3ElementModel)

M3BoardModel::M3BoardModel()
{
	
}

M3BoardModel::~M3BoardModel()
{
}

void M3BoardModel::Init() {
	SUBSCRIBE_PROP(M3BoardEntity, Cols)
	SUBSCRIBE_PROP(M3BoardEntity, Rows)
	SUBSCRIBE_PROP(M3BoardEntity, Cells)
}

void M3BoardModel::Serialize() {

}

void M3BoardModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

	AM3BoardScheme* BoardScheme = static_cast<AM3BoardScheme*>(Scheme);

	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	Entity->Get()->Cols->Set(Cols);
	Entity->Get()->Rows->Set(Rows);
	Entity->Get()->Cells->Get()->resize(Cols * Rows, nullptr);

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {

			assert(BoardScheme->Cells[i + j * Cols]->Col == i);
			assert(BoardScheme->Cells[i + j * Cols]->Row == j);

			M3CellModel_SharedPtr CellModel = std::make_shared<M3CellModel>();
			CellModel->Init();
			CellModel->AddToContainer();
			CellModel->Entity->Get()->Col->Set(i);
			CellModel->Entity->Get()->Row->Set(j);

			AM3CellAppointmentScheme* Appointment = BoardScheme->Cells[i + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT);
			if (Appointment) {
				M3ElementModel_SharedPtr ElementModel = std::make_shared<M3ElementModel>();
				ElementModel->Init();
				ElementModel->AddToContainer();
				ElementModel->Entity->Get()->ElementId->Set(static_cast<int>(Appointment->Id));

				CellModel->AddSubmodel(ElementModel);
			}
			Entity->Get()->Cells->Get()->data()[i + j * Cols] = CellModel;
		}
	}
 	M3CellModel::ApplyContainer();
	M3ElementModel::ApplyContainer();
}

