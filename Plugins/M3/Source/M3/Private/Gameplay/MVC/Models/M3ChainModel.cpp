// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ChainModel.h"
#include "M3ElementModel.h"
#include "M3BoardModel.h"

M3ChainModel::M3ChainModel() {
}

M3ChainModel::~M3ChainModel() {
}

void M3ChainModel::Init() {

}

void M3ChainModel::Serialize() {

}

void M3ChainModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3ChainModel::DetectHorizontalMatches(const M3BoardModel_SharedPtr& BoardModel) {
	const int Cols = BoardModel->GetCols();
	const int Rows = BoardModel->GetRows();

	for (int Row = 0; Row < Rows; Row++) {
		for (int Col = 0; Col < Cols - 2;) {
			const auto ElementModel = BoardModel->GetElement(Col, Row);
			if (ElementModel && ElementModel->CanMatch() && ElementModel->GetColor() != EM3ElementColor::NONE) {
				EM3ElementColor Color = ElementModel->GetColor();
				if (BoardModel->GetElement(Col + 1, Row) &&
					BoardModel->GetElement(Col + 1, Row)->CanMatch() &&
					BoardModel->GetElement(Col + 1, Row)->GetColor() == Color &&
					BoardModel->GetElement(Col + 2, Row) &&
					BoardModel->GetElement(Col + 2, Row)->CanMatch() &&
					BoardModel->GetElement(Col + 2, Row)->GetColor() == Color) {

					const auto Chain = std::make_shared<M3Chain>();
					Chain->Chain = EM3Chain::HORIZONTAL;
					bool bIsMutableChain = false;
					do {
						Chain->Elements.push_back(BoardModel->GetElement(Col, Row));
						if (Col + 1 < Cols && BoardModel->GetElement(Col + 1, Row) &&
							BoardModel->GetElement(Col + 1, Row)->GetColor() == Color &&
							!BoardModel->GetElement(Col + 1, Row)->IsInIdle()) {

							bIsMutableChain = true;
							break;
						}
						Col++;
					} while (Col < Cols && BoardModel->GetElement(Col, Row) &&
						BoardModel->GetElement(Col, Row)->CanMatch() &&
						BoardModel->GetElement(Col, Row)->GetColor() == Color);
					if (!bIsMutableChain) {
						PushChain(Chain);
					}
					continue;
				}
			}
			Col++;
		}
	}
}

void M3ChainModel::DetectVerticalMatches(const M3BoardModel_SharedPtr& BoardModel) {
	const int Cols = BoardModel->GetCols();
	const int Rows = BoardModel->GetRows();

	for (int Col = 0; Col < Cols; Col++) {
		for (int Row = 0; Row < Rows - 2;) {
			const auto ElementModel = BoardModel->GetElement(Col, Row);
			if (ElementModel && ElementModel->CanMatch() && ElementModel->GetColor() != EM3ElementColor::NONE) {
				EM3ElementColor Color = ElementModel->GetColor();
				if (BoardModel->GetElement(Col, Row + 1) &&
					BoardModel->GetElement(Col, Row + 1)->CanMatch() &&
					BoardModel->GetElement(Col, Row + 1)->GetColor() == Color &&
					BoardModel->GetElement(Col, Row + 2) &&
					BoardModel->GetElement(Col, Row + 2)->CanMatch() &&
					BoardModel->GetElement(Col, Row + 2)->GetColor() == Color) {

					const auto Chain = std::make_shared<M3Chain>();
					Chain->Chain = EM3Chain::VERTICAL;
					bool bIsMutableChain = false;
					do {
						Chain->Elements.push_back(BoardModel->GetElement(Col, Row));
						if (Row + 1 < Rows && BoardModel->GetElement(Col, Row + 1) &&
							BoardModel->GetElement(Col, Row + 1)->GetColor() == Color &&
							!BoardModel->GetElement(Col, Row + 1)->IsInIdle()) {

							bIsMutableChain = true;
							break;
						}
						Row++;
					} while (Row < Rows && BoardModel->GetElement(Col, Row) &&
						BoardModel->GetElement(Col, Row)->CanMatch() &&
						BoardModel->GetElement(Col, Row)->GetColor() == Color);
					if (!bIsMutableChain) {
						PushChain(Chain);
					}
					continue;
				}
			}
			Row++;
		}
	}
}

void M3ChainModel::PushChain(const M3Chain_SharedPtr& Chain) {
	if (Chain->Chain == EM3Chain::HORIZONTAL) {
		const auto& Chains = Entity->Get()->HorizontalChains->Get();
		Chains->push_back(Chain);
		Entity->Get()->HorizontalChains->Set(Chains);
	}
	else if (Chain->Chain == EM3Chain::VERTICAL) {
		const auto& Chains = Entity->Get()->VerticalChains->Get();
		Chains->push_back(Chain);
		Entity->Get()->VerticalChains->Set(Chains);
	}
}

bool M3ChainModel::IsChainsExist() const {
	return Entity->Get()->HorizontalChains->Get()->size() != 0 || Entity->Get()->VerticalChains->Get()->size() != 0;
}

M3Chain_SharedPtr M3ChainModel::PopChain() {
	M3Chain_SharedPtr Chain = nullptr;
	if (Entity->Get()->HorizontalChains->Get()->size() != 0) {
		Chain = Entity->Get()->HorizontalChains->Get()->back();
		Entity->Get()->HorizontalChains->Get()->pop_back();
	} else if (Entity->Get()->VerticalChains->Get()->size() != 0) {
		Chain = Entity->Get()->VerticalChains->Get()->back();
		Entity->Get()->VerticalChains->Get()->pop_back();
	}

	return Chain;
}

