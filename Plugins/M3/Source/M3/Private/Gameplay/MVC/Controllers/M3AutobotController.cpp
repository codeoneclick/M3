// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3AutobotController.h"
#include "M3AutobotModel.h"
#include "M3SharedModel.h"
#include "M3SwapModel.h"
#include "M3CellModel.h"
#include "M3SuperElementModel.h"
#include "M3BoardStateModel.h"

FORWARD_DECL_STRONG(M3SuperElementModel)

M3AutobotController::M3AutobotController() {
}

M3AutobotController::~M3AutobotController() {
}

bool M3AutobotController::CanBeExecuted() const {
	const auto& AutobotModel = M3SharedModel::GetInstance()->GetSubmodel<M3AutobotModel>();
	const auto& BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
 	return AutobotModel->GetEnabled() && !BoardStateModel->IsSomeElementInAction();
}

void M3AutobotController::Execute(float Deltatime) {
	const auto& AutobotModel = M3SharedModel::GetInstance()->GetSubmodel<M3AutobotModel>();
	const auto& SwapModel = M3SharedModel::GetInstance()->GetSubmodel<M3SwapModel>();
	const auto& BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	const auto& PotentialSwaps = SwapModel->GetPotentialSwaps();
	float SwapMaxWeightning = 0.f;
	size_t SwapMaxWeightingIndex = std::numeric_limits<size_t>::max();
	for (size_t i = 0; i < PotentialSwaps->size(); ++i) {
		const auto& PotentialSwap = PotentialSwaps->data()[i];
		if (PotentialSwap->GetWeightning() > SwapMaxWeightning) {
			SwapMaxWeightning = PotentialSwap->GetWeightning();
			SwapMaxWeightingIndex = i;
		}
	}

	M3SuperElementModel_SharedPtr SuperElementModelToUse = nullptr;
	const auto& SuperElements = M3SuperElementModel::Container()->Get();
	for (auto It = SuperElements->begin(); It != SuperElements->end(); ++It) {
		const auto& ElementModel = (*It)->GetParent<M3ElementModel>();
		if (ElementModel && ElementModel->CanMatch()) {
			const auto& CellModel = ElementModel->GetParent<M3CellModel>();
			if (CellModel) {
				const auto& SuperElementModel = std::static_pointer_cast<M3SuperElementModel>((*It));
				int ColToSwap = 0;
				int RowToSwap = 0;
				int SwapSuperelementMaxWeightning = AutobotModel->GetSuperElementWeightning(SuperElementModel->GetId(), CellModel->GetCol(), CellModel->GetRow(), &ColToSwap, &RowToSwap);
				if (SwapSuperelementMaxWeightning > SwapMaxWeightning) {
					SwapMaxWeightning = SwapSuperelementMaxWeightning;
					SuperElementModelToUse = SuperElementModel;
				}
			}
		}
	}

	if (SuperElementModelToUse) {
		const auto& ElementModel = SuperElementModelToUse->GetParent<M3ElementModel>();
		ElementModel->SetState(EM3ElementState::MATCHING);
		BoardStateModel->IncGameTurn();
	} else if (SwapMaxWeightingIndex != std::numeric_limits<size_t>::max()) {
		const auto& PotentialSwap = PotentialSwaps->data()[SwapMaxWeightingIndex];
		SwapModel->AddSwapElement(PotentialSwap->GetSwapElementA());
		SwapModel->AddSwapElement(PotentialSwap->GetSwapElementB());
		PotentialSwap->GetSwapElementA()->SetState(EM3ElementState::SWAPPING);
		PotentialSwap->GetSwapElementB()->SetState(EM3ElementState::SWAPPING);
		if (SwapModel->IsPossibleToSwap()) {
			SwapModel->SwapElements(PotentialSwap->GetSwapElementA(), PotentialSwap->GetSwapElementB());
		}
		SwapModel->Entity->Get()->SwapElementA->Set(nullptr);
		SwapModel->Entity->Get()->SwapElementB->Set(nullptr);
	}
}
