// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementsMatchController.h"
#include "M3ChainModel.h"
#include "M3ElementModel.h"
#include "M3BoardSettingsModel.h"
#include "M3BoardStateModel.h"
#include "M3SharedModel.h"

M3ElementsMatchController::M3ElementsMatchController() {
	LastExecutedTurn = 0;
}

M3ElementsMatchController::~M3ElementsMatchController() {
}

bool M3ElementsMatchController::CanBeExecuted() const {
	const auto ChainModel = M3SharedModel::GetInstance()->GetSubmodel<M3ChainModel>();
	return ChainModel->IsChainsExist();
}

void M3ElementsMatchController::Execute(float Deltatime) {
	const auto ChainModel = M3SharedModel::GetInstance()->GetSubmodel<M3ChainModel>();
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();

	while (ChainModel->IsChainsExist()) {
		const auto Chain = ChainModel->PopChain();
		for (const auto Element : Chain->Elements) {
			if (!Element->IsInState(EM3ElementState::REMOVING) &&
				!Element->IsInState(EM3ElementState::MATCHING)) {
				Element->SetState(EM3ElementState::MATCHING);
			}
		}
		float ComboMatchScoresMultiplier = 1;
		if (LastExecutedTurn == BoardStateModel->GetTurn() && BoardSettingsModel->GetIsUseComboMatchScores()) {
			ComboMatchScoresMultiplier = BoardSettingsModel->GetComboMatchScoresMultiplier();
		} 
		BoardStateModel->Entity->Get()->Scores->Set(BoardStateModel->Entity->Get()->Scores->Get() + Chain->Elements.size() * BoardSettingsModel->GetOneMatchScores() * ComboMatchScoresMultiplier);
	}
	LastExecutedTurn = BoardStateModel->GetTurn();
}
