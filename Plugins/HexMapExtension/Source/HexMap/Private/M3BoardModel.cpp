// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3Scheme.h"
#include "M3BoardStateModel.h"
#include "M3SharedModel.h"

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

void M3BoardModel::Deserialize(UM3Scheme_INTERFACE* Scheme) {

	UM3BoardScheme* BoardScheme = static_cast<UM3BoardScheme*>(Scheme);

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

			UM3CellAppointmentScheme* Appointment = BoardScheme->Cells[i + j * Cols]->GetAppointment(EM3CellAppointment::ELEMENT);
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

M3CellModel_SharedPtr M3BoardModel::GetCell(int Col, int Row) const {
	M3CellModel_SharedPtr CellModel = nullptr;
	int Index = Col + Row * GetCols();
	if (Index < Entity->Get()->Cells->Get()->size()) {
		CellModel = Entity->Get()->Cells->Get()->data()[Index];
	}
	return CellModel;
}

M3ElementModel_SharedPtr M3BoardModel::GetElement(int Col, int Row) const {
	M3ElementModel_SharedPtr ElementModel = nullptr;
	M3CellModel_SharedPtr CellModel = GetCell(Col, Row);
	if (CellModel) {
		ElementModel = CellModel->GetSubmodel<M3ElementModel>();
	}
	return ElementModel;
}

void M3BoardModel::CreateHole(int Col, int Row) {
	const auto Element = GetElement(Col, Row);
	if (Element) {
		const auto Cell = Element->GetParent<M3CellModel>();
		Cell->RemoveSubmodel(Element);

		const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
		BoardStateModel->PushUnusedElement(Element);
		BoardStateModel->SetIsHolesExist(true);
	}
}

void M3BoardModel::RemoveMatched() {
	const int Cols = GetCols();
	const int Rows = GetRows();
	for (int Col = 0; Col < Cols; ++Col) {
		for (int Row = 0; Row < Rows; ++Row) {
			const auto Element = GetElement(Col, Row);
			if (Element && Element->IsInState(EM3ElementState::REMOVING)) {
				CreateHole(Col, Row);
			}
		}
	}
}

void M3BoardModel::MoveElement(const M3CellModel_SharedPtr& From, const M3CellModel_SharedPtr& To) {
	const auto Element = From->GetSubmodel<M3ElementModel>();
	From->RemoveSubmodel(Element);
	To->AddSubmodel(Element);
}

