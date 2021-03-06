// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementsSpawnController.h"
#include "M3BoardStateModel.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3RegularelementModel.h"
#include "M3CellModel.h"
#include "M3SharedModel.h"
#include "M3BoardSettingsModel.h"
#include "M3Scheme.h"

M3ElementsSpawnController::M3ElementsSpawnController() {
}

M3ElementsSpawnController::~M3ElementsSpawnController() {
}

bool M3ElementsSpawnController::CanBeExecuted() const {
	const auto BoardState = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	return BoardState->GetIsHolesExist();
}

void M3ElementsSpawnController::Execute(float Deltatime) {
	const auto& Board = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	const auto& BoardSettings = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	int PreviousElementId = -1;

	const int Cols = Board->GetCols();
	const int Rows = Board->GetRows();

	for (int Col = 0; Col < Cols; ++Col) {
		const auto& Cell = Board->GetCell(Col, Rows - 1);
		if (Cell && Cell->CanContainElement()) {
			const auto Element = M3ElementModel::Construct<M3ElementModel>();
			const auto Regularelement = M3RegularelementModel::Construct<M3RegularelementModel>();
			Element->AddSubmodel(Regularelement);
			Cell->AddSubmodel(Element);
			int ElementRandomId = -1;
			do {
				int ElementRandomIndex = FMath::RandRange(0, BoardSettings->GetElementIds()->size() - 1);
				ElementRandomId = static_cast<int>(BoardSettings->GetElementIds()->data()[ElementRandomIndex]);
			} while (ElementRandomId == PreviousElementId);

			PreviousElementId = ElementRandomId;

			Regularelement->Entity->Get()->Id->Set(static_cast<EM3ElementId>(ElementRandomId));

			if (!Element->Entity->Get()->IsAssignedToView->Get()) {
				M3ElementModel::ApplyContainer();
			}

			if (!Regularelement->Entity->Get()->IsAssignedToView->Get()) {
				M3RegularelementModel::ApplyContainer();
			}

			Element->SetState(EM3ElementState::SPAWNING);
		}
	}
}
