// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3GameStateController.h"
#include "M3BoardStateModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"

M3GameStateController::M3GameStateController() {
	InGameDuration = 0.f;
}

M3GameStateController::~M3GameStateController() {
}

bool M3GameStateController::CanBeExecuted() const {
	return true;
}

void M3GameStateController::Execute(float Deltatime) {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	if (!BoardSettingsModel->GetIsTurnBased()) {
		InGameDuration += Deltatime;
		const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
		BoardStateModel->Entity->Get()->Duration->Set(FMath::RoundToInt(InGameDuration));
	}
}