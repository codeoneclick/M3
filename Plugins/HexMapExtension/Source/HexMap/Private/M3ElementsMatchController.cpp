// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementsMatchController.h"
#include "M3ChainModel.h"
#include "M3ElementModel.h"
#include "M3SharedModel.h"

M3ElementsMatchController::M3ElementsMatchController() {
}

M3ElementsMatchController::~M3ElementsMatchController() {
}

bool M3ElementsMatchController::CanBeExecuted() const {
	const auto ChainModel = M3SharedModel::GetInstance()->GetSubmodel<M3ChainModel>();
	return ChainModel->IsChainsExist();
}

void M3ElementsMatchController::Execute(float Deltatime) {
	const auto ChainModel = M3SharedModel::GetInstance()->GetSubmodel<M3ChainModel>();

	while (ChainModel->IsChainsExist()) {
		const auto Chain = ChainModel->PopChain();
		for (const auto Element : Chain->Elements) {
			if (!Element->IsInState(EM3ElementState::REMOVING) &&
				!Element->IsInState(EM3ElementState::MATCHING)) {
				Element->SetState(EM3ElementState::MATCHING);
			}
		}
	}
}
