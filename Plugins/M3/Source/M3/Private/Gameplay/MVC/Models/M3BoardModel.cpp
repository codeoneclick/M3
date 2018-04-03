// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3RegularelementModel.h"
#include "M3SuperelementModel.h"
#include "M3Scheme.h"
#include "M3BoardStateModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"

FORWARD_DECL_STRONG(M3CellModel)
FORWARD_DECL_STRONG(M3ElementModel)
FORWARD_DECL_STRONG(M3RegularelementModel)
FORWARD_DECL_STRONG(M3SuperelementModel)

M3BoardModel::M3BoardModel() {
	
}

M3BoardModel::~M3BoardModel() {
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
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	int Cols = BoardScheme->Cols;
	int Rows = BoardScheme->Rows;

	Entity->Get()->Cols->Set(Cols);
	Entity->Get()->Rows->Set(Rows);
	Entity->Get()->Cells->Get()->resize(Cols * Rows, nullptr);
	const auto Cells = Entity->Get()->Cells->Get()->data();

	std::vector<M3ElementModel_SharedPtr> RandomElements;

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {

			assert(BoardScheme->Cells[i + j * Cols]->Col == i);
			assert(BoardScheme->Cells[i + j * Cols]->Row == j);

			M3CellModel_SharedPtr CellModel = M3CellModel::Construct<M3CellModel>();
			CellModel->Entity->Get()->Col->Set(i);
			CellModel->Entity->Get()->Row->Set(j);

			if (BoardScheme->Cells[i + j * Cols]->IsAppointmentExist(EM3CellAppointment::REGULARELEMENT) ||
				BoardScheme->Cells[i + j * Cols]->IsAppointmentExist(EM3CellAppointment::SUPERELEMENT)) {
				
				M3ElementModel_SharedPtr ElementModel = M3ElementModel::Construct<M3ElementModel>();
				
				AM3CellAppointmentScheme* Appointment = BoardScheme->Cells[i + j * Cols]->GetAppointment(EM3CellAppointment::REGULARELEMENT);
				if (Appointment) {
					M3RegularelementModel_SharedPtr RegularelementModel = M3RegularelementModel::Construct<M3RegularelementModel>();
					RegularelementModel->Entity->Get()->Id->Set(Appointment->Id);
					ElementModel->AddSubmodel(RegularelementModel);
					CellModel->AddSubmodel(ElementModel);
				}

				Appointment = BoardScheme->Cells[i + j * Cols]->GetAppointment(EM3CellAppointment::SUPERELEMENT);
				if (Appointment) {

				}
			}
			if (BoardScheme->Cells[i + j * Cols]->IsAppointmentExist(EM3CellAppointment::FUNCTIONAL)) {
				
				AM3CellAppointmentScheme* Appointment = BoardScheme->Cells[i + j * Cols]->GetAppointment(EM3CellAppointment::FUNCTIONAL);
				if (Appointment) {
					if (Appointment->Id == EM3ElementId::CELL_CLOSED) {
						CellModel->Entity->Get()->IsClosed->Set(true);
					}
					if (Appointment->Id == EM3ElementId::CELL_HOLE) {
						CellModel->Entity->Get()->IsClosed->Set(false);
					}
					if (Appointment->Id == EM3ElementId::CELL_RANDOM) {
						M3ElementModel_SharedPtr ElementModel = M3ElementModel::Construct<M3ElementModel>();
						M3RegularelementModel_SharedPtr RegularelementModel = M3RegularelementModel::Construct<M3RegularelementModel>();
						ElementModel->AddSubmodel(RegularelementModel);
						CellModel->AddSubmodel(ElementModel);
						RandomElements.push_back(ElementModel);
					}
				}
			}
			Cells[i + j * Cols] = CellModel;
		}
	}

	for (const auto RandomElement : RandomElements) {
		int Col = RandomElement->GetParent<M3CellModel>()->GetCol();
		int Row = RandomElement->GetParent<M3CellModel>()->GetRow();

		EM3ElementId ElementId = EM3ElementId::UNKNOWN;
		do {
			int ElementIdIndex = FMath::RandRange(0, BoardSettingsModel->GetElementIds()->size() - 1);
			ElementId = BoardSettingsModel->GetElementIds()->data()[ElementIdIndex];
		} while ((Col >= 2 &&
				Cells[(Col - 1) + Row * Cols]->IsContainElement() && 
				Cells[(Col - 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() && 
				Cells[(Col - 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[(Col - 2) + Row * Cols]->IsContainElement() &&
				Cells[(Col - 2) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[(Col - 2) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId)
				||
				(Col < Cols - 2 &&
				Cells[(Col + 1) + Row * Cols]->IsContainElement() &&
				Cells[(Col + 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[(Col + 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[(Col + 2) + Row * Cols]->IsContainElement() &&
				Cells[(Col + 2) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[(Col + 2) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId)
				||
				(Col >= 1 && Col < Cols - 1 &&
				Cells[(Col - 1) + Row * Cols]->IsContainElement() &&
				Cells[(Col - 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[(Col - 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[(Col + 1) + Row * Cols]->IsContainElement() &&
				Cells[(Col + 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[(Col + 1) + Row * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId)
				||
				(Row >= 2 &&
				Cells[Col + (Row - 1) * Cols]->IsContainElement() &&
				Cells[Col + (Row - 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row - 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[Col + (Row - 2) * Cols]->IsContainElement() &&
				Cells[Col + (Row - 2) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row - 2) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId)
				||
				(Row < Rows - 2 &&
				Cells[Col + (Row + 1) * Cols]->IsContainElement() &&
				Cells[Col + (Row + 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row + 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[Col + (Row + 2) * Cols]->IsContainElement() &&
				Cells[Col + (Row + 2) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row + 2) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId)
				||
				(Row >= 1 && Row < Rows - 1 &&
				Cells[Col + (Row - 1) * Cols]->IsContainElement() &&
				Cells[Col + (Row - 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row - 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId &&
				Cells[Col + (Row + 1) * Cols]->IsContainElement() &&
				Cells[Col + (Row + 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>() &&
				Cells[Col + (Row + 1) * Cols]->GetSubmodel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>()->GetId() == ElementId));

		RandomElement->GetSubmodel<M3RegularelementModel>()->Entity->Get()->Id->Set(ElementId);
	}

 	M3CellModel::ApplyContainer();
	M3ElementModel::ApplyContainer();
	M3RegularelementModel::ApplyContainer();
	M3SuperelementModel::ApplyContainer();
}

M3CellModel_SharedPtr M3BoardModel::GetCell(int Col, int Row) const {
	M3CellModel_SharedPtr CellModel = nullptr;
	size_t Index = Col + Row * GetCols();
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
		Element->Reset();
		ensure(Element->GetParent<M3CellModel>() == nullptr);
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

