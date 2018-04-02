// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementsDropController.h"
#include "M3BoardStateModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3BoardModel.h"
#include "M3SharedModel.h"

M3ElementsDropController::M3ElementsDropController() {
}

M3ElementsDropController::~M3ElementsDropController() {
}

bool M3ElementsDropController::DropElement(const M3CellModel_SharedPtr& From, const M3CellModel_SharedPtr& To) {
	bool Result = false;
	const auto Element = From->GetSubmodel<M3ElementModel>();
	if (Element && Element->CanDrop()) {
		M3BoardModel::MoveElement(From, To);
		Element->SetState(EM3ElementState::DROPPING);
		Result = true;
	}
	return Result;
}

bool M3ElementsDropController::DropUpDown(const M3BoardModel_SharedPtr& Board, const M3CellModel_SharedPtr& Cell, int Col, int Row) {
	bool Result = false;
	for (int Lookup = Row + 1; Lookup < Board->GetRows(); ++Lookup) {
		const auto LookupCell = Board->GetCell(Col, Lookup);
		if (LookupCell && LookupCell->IsContainElement()) {
			Result = DropElement(LookupCell, Cell);
			const auto Element = Board->GetElement(Col, Lookup);
			if (Result || (Element && !Element->CanDrop())) {
				break;
			}
		}
		else if (!LookupCell || (LookupCell && !LookupCell->CanContainElement())) {
			break;
		}
	}
	return Result;
}

bool M3ElementsDropController::DropDiagonal(const M3BoardModel_SharedPtr& Board, int Col, int Row) {
	bool Result = false;
	int Lookup = Row + 1;
	auto LookupCell = Board->GetCell(Col, Lookup);
	bool bIsBlocked = !LookupCell;
	if (!bIsBlocked) {
		for (Lookup = Row + 1; Lookup < Board->GetRows(); ++Lookup) {
			LookupCell = Board->GetCell(Col, Lookup);
			if (!LookupCell) {
				break;
			}
			else {
				bIsBlocked = !LookupCell->IsContainElement() && !LookupCell->CanContainElement();
				if (bIsBlocked) {
					break;
				}
			}
			const auto Element = LookupCell->GetSubmodel<M3ElementModel>();
			if (Element) {
				if (Element->IsInState(EM3ElementState::DROPPING)) {
					break;
				}
				bIsBlocked = Element->IsDropBlocked();
				if (bIsBlocked) {
					break;
				}
			}
		}
	}
	int LockedLookup = Lookup + 1;
	M3CellModel_SharedPtr From = nullptr;
	M3CellModel_SharedPtr To = nullptr;
	if (bIsBlocked) {
		for (Lookup = Row + 1; Lookup < LockedLookup; ++Lookup) {
			const auto CellTemp = Board->GetCell(Col, Lookup - 1);
			if (Col - 1 >= 0) {
				LookupCell = Board->GetCell(Col - 1, Lookup);
				if (LookupCell && LookupCell->IsContainElement() && CellTemp && CellTemp->CanContainElement()) {
					const auto& Element = Board->GetElement(Col - 1, Lookup);
					if (Element && !Element->IsDropBlocked()) {
						From = LookupCell;
						To = CellTemp;
					}
				}
			}
			if (Col + 1 < Board->GetCols()) {
				LookupCell = Board->GetCell(Col + 1, Lookup);
				if (LookupCell && LookupCell->IsContainElement() && CellTemp && CellTemp->CanContainElement()) {
					const auto& Element = Board->GetElement(Col + 1, Lookup);
					if (Element && !Element->IsDropBlocked()) {
						From = LookupCell;
						To = CellTemp;
					}
				}
			}
		}
	}

	if (From && To) {
		Result = DropElement(From, To);
	}

	return Result;
}

bool M3ElementsDropController::CanBeExecuted() const {
	const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	return BoardStateModel->GetIsHolesExist();
}

void M3ElementsDropController::Execute(float Deltatime) {
	const auto& Board = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	const int Cols = Board->GetCols();
	const int Rows = Board->GetRows();

	std::vector<int> InactiveCols;
	for (int Col = 0; Col < Cols; Col++) {
		for (int Row = 0; Row < Rows; Row++) {
			const auto& Cell = Board->GetCell(Col, Row);
			if (Cell && Cell->CanContainElement()) {
				if (DropUpDown(Board, Cell, Col, Row)) {
					continue;
				}
				else {
					InactiveCols.push_back(Col);
				}
			}
		}
	}
	for (const auto Col : InactiveCols) {
		for (int Row = 0; Row < Rows; Row++) {
			const auto& Cell = Board->GetCell(Col, Row);
			if (Cell && Cell->CanContainElement()) {
				if (DropDiagonal(Board, Col, Row)) {
					break;
				}
			}
		}
	}
}
