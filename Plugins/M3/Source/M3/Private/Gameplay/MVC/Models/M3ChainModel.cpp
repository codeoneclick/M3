// Copyright serhii serhiiv 2018 All rights reserved.

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
						Chain->Elements.insert(BoardModel->GetElement(Col, Row));
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
						Chain->Elements.insert(BoardModel->GetElement(Col, Row));
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

	bool IsSomeElementContainsInChain = false;
	const auto SuperelementChains = Entity->Get()->SuperelementChains->Get();
	for (int i = 0; i < SuperelementChains->size(); ++i) {
		const auto SuperelementChain = SuperelementChains->data()[i];

		std::set<M3ElementModel_SharedPtr> Intersection;
		std::set_intersection(Chain->Elements.begin(), Chain->Elements.end(), SuperelementChain->Elements.begin(), SuperelementChain->Elements.end(), std::inserter(Intersection, Intersection.begin()));
		if (Intersection.size() > 0) {
			IsSomeElementContainsInChain = true;
			for (const auto Element : Chain->Elements) {
				SuperelementChain->Elements.insert(Element);
			}
		}
	}

	if (!IsSomeElementContainsInChain) {
		SuperelementChains->push_back(Chain);
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

void M3ChainModel::ValidateElementChains(const M3ElementModel_SharedPtr& Element) {
	const auto HorizontalChains = Entity->Get()->HorizontalChains->Get();
	const auto VerticalChains = Entity->Get()->VerticalChains->Get();

	for (int i = 0; i < HorizontalChains->size(); ++i) {
		const auto HorizontalChain = HorizontalChains->data()[i];
		HorizontalChain->Elements.erase(Element);
	}

	for (int i = 0; i < VerticalChains->size(); ++i) {
		const auto VerticalChain = VerticalChains->data()[i];
		VerticalChain->Elements.erase(Element);
	}
}

void M3ChainModel::ValidateSuperelementChains() {
	auto SuperelementChains = Entity->Get()->SuperelementChains->Get();
	for (auto It = SuperelementChains->begin(); It != SuperelementChains->end();) {
		if ((*It)->Elements.size() <= 3) {
			It = SuperelementChains->erase(It);
		} else {
			++It;
		}
	}
	for (auto It = SuperelementChains->begin(); It != SuperelementChains->end(); ++It) {
		ensure((*It)->Elements.size() > 3);
	}

	/*
	while (1) {
		M3Chain_SharedPtr IntersectedSuperelementChainA = nullptr;
		M3Chain_SharedPtr IntersectedSuperelementChainB = nullptr;
		for (int i = 0; i < SuperelementChains->size(); ++i) {
			const auto SuperelementChain_ItA = SuperelementChains->data()[i];
			std::sort(SuperelementChain_ItA->Elements.begin(), SuperelementChain_ItA->Elements.end());
			for (int j = 0; j < SuperelementChains->size(); ++j) {
				const auto SuperelementChain_ItB = SuperelementChains->data()[j];
				std::sort(SuperelementChain_ItB->Elements.begin(), SuperelementChain_ItB->Elements.end());
				if (SuperelementChain_ItA != SuperelementChain_ItB) {

					std::vector<M3ElementModel_SharedPtr> Intersection;
					std::set_intersection(SuperelementChain_ItA->Elements.begin(), SuperelementChain_ItA->Elements.end(), SuperelementChain_ItB->Elements.begin(), SuperelementChain_ItB->Elements.end(), std::back_inserter(Intersection));
					if (Intersection.size() > 0) {
						IntersectedSuperelementChainA = SuperelementChain_ItA;
						IntersectedSuperelementChainB = SuperelementChain_ItB;
						break;
					}
				}
			}
			if (IntersectedSuperelementChainA && IntersectedSuperelementChainB) {
				break;
			}
		}
		if (IntersectedSuperelementChainA && IntersectedSuperelementChainB) {
			const auto IntersectedSuperelementChainItA = std::find(SuperelementChains->begin(), SuperelementChains->end(), IntersectedSuperelementChainA);
			const auto IntersectedSuperelementChainItB = std::find(SuperelementChains->begin(), SuperelementChains->end(), IntersectedSuperelementChainB);
			for (const auto Element : IntersectedSuperelementChainB->Elements) {
				if (std::find(IntersectedSuperelementChainA->Elements.begin(), IntersectedSuperelementChainA->Elements.end(), Element) != IntersectedSuperelementChainA->Elements.end()) {
					IntersectedSuperelementChainA->Elements.push_back(Element);
				}
			}
			SuperelementChains->erase(IntersectedSuperelementChainItB);
		}
		else {
			break;
		}
	}*/
}

bool M3ChainModel::IsSuperelementChainsExist() const {
	return Entity->Get()->SuperelementChains->Get()->size() != 0;
}

M3Chain_SharedPtr M3ChainModel::PopSuperelementChain() {
	M3Chain_SharedPtr Chain = nullptr;
	if (Entity->Get()->SuperelementChains->Get()->size() != 0) {
		Chain = Entity->Get()->SuperelementChains->Get()->back();
		Entity->Get()->SuperelementChains->Get()->pop_back();
	}
	return Chain;
}

