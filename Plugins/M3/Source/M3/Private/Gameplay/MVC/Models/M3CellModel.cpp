// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3CellModel.h"
#include "M3ElementModel.h"

M3CellModel::M3CellModel() {

}

M3CellModel::~M3CellModel() {

}

void M3CellModel::Init() {

}

void M3CellModel::Serialize() {

}

void M3CellModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

bool M3CellModel::IsContainElement() {
	bool Result = false;
	const auto Element = GetSubmodel<M3ElementModel>();
	if (Element && !GetIsClosed()) {
		Result = true;
	}
	return Result;
}

bool M3CellModel::CanContainElement() {
	bool Result = false;
	if (!IsContainElement() && !GetIsClosed()) {
		Result = true;
	}
	return Result;
}

bool M3CellModel::IsNeighbours(const M3CellModel_SharedPtr& CurrentCellModel, const M3CellModel_SharedPtr& NeighbourCellModel) {
	const int hDist = FMath::Abs(CurrentCellModel->GetCol() - NeighbourCellModel->GetCol());
	const int vDist = FMath::Abs(CurrentCellModel->GetRow() - NeighbourCellModel->GetRow());

	return (hDist == 1 && vDist == 0) || (hDist == 0 && vDist == 1);
}
