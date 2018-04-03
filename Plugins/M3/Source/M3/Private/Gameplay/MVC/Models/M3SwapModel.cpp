// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SwapModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3BoardModel.h"
#include "M3SharedModel.h"

M3PotentialSwapModel::M3PotentialSwapModel() {
}

M3PotentialSwapModel::~M3PotentialSwapModel() {
}

void M3PotentialSwapModel::Serialize() {
}

void M3PotentialSwapModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
}

void M3PotentialSwapModel::Init() {
}

M3SwapModel::M3SwapModel() {
}

M3SwapModel::~M3SwapModel() {
}

void M3SwapModel::Serialize() {
}

void M3SwapModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
}

void M3SwapModel::Init() {
}

bool M3SwapModel::CanSwapElementWithElementColor(const std::shared_ptr<std::vector<M3CellModel_SharedPtr>>& Cells, int Cols, int Rows, int Col, int Row, EM3ElementColor Color) {
	bool Result = false;
	const auto& Cell = Cells->data()[Col + Row * Cols];
	if (Cell) {
		const auto& Element = Cell->GetSubmodel<M3ElementModel>();
		if (Element) {
			Result = Element->GetColor() == Color;
		}
	}
	return Result;
}

bool M3SwapModel::CanSwapElements(const M3ElementModel_SharedPtr& ElementA, const M3ElementModel_SharedPtr& ElementB) {
	bool Result = false;
	const auto CellA = ElementA->GetParent<M3CellModel>();
	const auto CellB = ElementB->GetParent<M3CellModel>();

	const int swapHorizontal = FMath::Abs(CellA->Entity->Get()->Col->Get() - CellB->Entity->Get()->Col->Get());
	const int swapVertical = FMath::Abs(CellA->Entity->Get()->Row->Get() - CellB->Entity->Get()->Row->Get());
	if ((swapHorizontal == 1 && swapVertical == 0) || (swapVertical == 1 && swapHorizontal == 0)) {
		Result = true;
	}
	return Result;
}

void M3SwapModel::SwapElements(const M3ElementModel_SharedPtr& ElementA, const M3ElementModel_SharedPtr& ElementB) {
	const auto& CellA = ElementA->GetParent<M3CellModel>();
	const auto& CellB = ElementB->GetParent<M3CellModel>();
	M3SwapModel::SwapElements(CellA, CellB);
}

void M3SwapModel::SwapElements(const M3CellModel_SharedPtr& CellA, const M3CellModel_SharedPtr& CellB) {
	const auto ElementA = CellA->GetSubmodel<M3ElementModel>();
	const auto ElementB = CellB->GetSubmodel<M3ElementModel>();

	if (ElementA != ElementB) {
		CellA->RemoveSubmodel(ElementA);
		CellB->RemoveSubmodel(ElementB);
		CellA->AddSubmodel(ElementB);
		CellB->AddSubmodel(ElementA);
	}
}

bool M3SwapModel::HasChain(const std::shared_ptr<std::vector<M3CellModel_SharedPtr>>& Cells, int Cols, int Rows, int Col, int Row) {
	const auto& Element = Cells->data()[Col + Row * Cols]->GetSubmodel<M3ElementModel>();
	int Count = 0;
	EM3ElementColor Color = Element->GetColor();

	int HorizontalStart = -1;
	int HorizontalEnd = -1;
	int HorizontalLength = 1;
	int i = -1;
	int j = -1;
	for (i = Col - 1; i >= 0 && M3SwapModel::CanSwapElementWithElementColor(Cells, Cols, Rows, i, Row, Color); --i, ++HorizontalLength);
	for (j = Col + 1; j < Cols && M3SwapModel::CanSwapElementWithElementColor(Cells, Cols, Rows, j, Row, Color); ++j, ++HorizontalLength);
	if (HorizontalLength >= 3) {
		Count += HorizontalLength;
		assert(j - (i + 1) == HorizontalLength);
		HorizontalStart = (i + 1);
		HorizontalEnd = j;
	}

	int VerticalStart = -1;
	int VerticalEnd = -1;
	int VerticalLength = 1;
	i = -1;
	j = -1;
	for (i = Row - 1; i >= 0 && M3SwapModel::CanSwapElementWithElementColor(Cells, Cols, Rows, Col, i, Color); --i, ++VerticalLength);
	for (j = Row + 1; j < Rows && M3SwapModel::CanSwapElementWithElementColor(Cells, Cols, Rows, Col, j, Color); ++j, ++VerticalLength);
	if (VerticalLength >= 3) {
		Count += VerticalLength;
		assert(j - (i + 1) == = VerticalLength);
		VerticalStart = (i + 1);
		VerticalEnd = j;
	}

	return Count >= 3;
}

void M3SwapModel::GeneratePotentialSwaps() {
	const auto& Board = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();

	const int Cols = Board->GetCols();
	const int Rows = Board->GetRows();

	const auto& Cells = Board->GetCells();
	const auto PotentialSwaps = std::make_shared<std::vector<M3PotentialSwapModel_SharedPtr>>();
	for (int Col = 0; Col < Cols; Col++) {
		for (int Row = 0; Row < Rows; Row++) {
			const auto Cell = Cells->data()[Col + Row * Cols];
			if (Cell) {
				const auto Element = Cell->GetSubmodel<M3ElementModel>();
				if (Element) {
					if (Col < Cols - 1) {
						const auto OtherCell = Cells->data()[(Col + 1) + Row * Cols];
						if (OtherCell) {
							const auto OtherElement = OtherCell->GetSubmodel<M3ElementModel>();
							if (OtherElement) {
								M3SwapModel::SwapElements(Cell, OtherCell);

								int ColTo = Col + 1;
								int RowTo = Row;
								bool Result = HasChain(Cells, Cols, Rows, Col + 1, Row);
								if (!Result) {
									ColTo = Col;
									Result = HasChain(Cells, Cols, Rows, Col, Row);
								}
								if (Result && M3SwapModel::CanSwapElements(Element, OtherElement)) {
									const auto PotentialSwapModel = std::make_shared<M3PotentialSwapModel>();
									PotentialSwapModel->SetSwapElementA(Element);
									PotentialSwapModel->SetSwapElementB(OtherElement);
									PotentialSwaps->push_back(PotentialSwapModel);
								}

								M3SwapModel::SwapElements(Cell, OtherCell);
							}
						}
					}

					if (Row < Rows - 1) {
						const auto OtherCell = Cells->data()[Col + (Row + 1) * Cols];
						if (OtherCell) {
							const auto OtherElement = OtherCell->GetSubmodel<M3ElementModel>();
							if (OtherElement) {
								M3SwapModel::SwapElements(Cell, OtherCell);

								int ColTo = Col;
								int RowTo = Row + 1;
								bool Result = HasChain(Cells, Cols, Rows, Col, Row + 1);
								if (!Result) {
									RowTo = Row;
									Result = HasChain(Cells, Cols, Rows, Col, Row);
								}
								if (Result && M3SwapModel::CanSwapElements(Element, OtherElement)) {
									const auto PotentialSwapModel = std::make_shared<M3PotentialSwapModel>();
									PotentialSwapModel->SetSwapElementA(Element);
									PotentialSwapModel->SetSwapElementB(OtherElement);
									PotentialSwaps->push_back(PotentialSwapModel);
								}

								M3SwapModel::SwapElements(Cell, OtherCell);
							}
						}
					}
				}
			}
		}
	}
	Entity->Get()->PotentialSwaps->Set(PotentialSwaps);
}

void M3SwapModel::AddSwapElement(const M3ElementModel_SharedPtr& Element) {
	const auto SwapElementA = GetSwapElementA();
	if (SwapElementA != nullptr &&
		SwapElementA != Element) {
		if (M3SwapModel::CanSwapElements(SwapElementA, Element)) {
			Entity->Get()->SwapElementB->Set(Element);
		} else {
			Entity->Get()->SwapElementA->Set(Element);
		}
	} else {
		Entity->Get()->SwapElementA->Set(Element);
	}
}

bool M3SwapModel::IsPossibleToSwap() const {
	bool Result = false;
	const auto SwapElementA = GetSwapElementA();
	const auto SwapElementB = GetSwapElementB();

	if (SwapElementA != nullptr && SwapElementB != nullptr) {
		const auto& PotentialSwaps = GetPotentialSwaps();
		std::for_each(PotentialSwaps->begin(), PotentialSwaps->end(), [&](const M3PotentialSwapModel_SharedPtr& PotentialSwap) {
			const auto PotentialSwapElementA = PotentialSwap->GetSwapElementA();
			const auto PotentialSwapElementB = PotentialSwap->GetSwapElementB();
			if ((SwapElementA == PotentialSwapElementA && SwapElementB == PotentialSwapElementB) ||
				(SwapElementB == PotentialSwapElementA && SwapElementA == PotentialSwapElementB)) {
				Result = true;
				return;
			}
		});
	}
	return Result;
}