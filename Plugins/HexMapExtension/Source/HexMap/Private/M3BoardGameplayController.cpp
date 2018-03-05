// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardGameplayController.h"
#include "M3AppEvent.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3CellModel.h"
#include "M3SwapModel.h"
#include "M3BoardActionsAccumulationModel.h"
#include "M3SharedModel.h"

M3BoardGameplayController::M3BoardGameplayController() {
	std::shared_ptr<M3AppEvent_Callback<int>> OnGameStartedCallback = std::make_shared<M3AppEvent_Callback<int>>(std::bind(&M3BoardGameplayController::OnGameStarted, this));
	std::shared_ptr<M3AppEvent<int>> OnGameStartedEvent = std::make_shared<M3AppEvent<int>>(M3Events::ON_GAME_STARTED, OnGameStartedCallback);
	Subscribe(OnGameStartedEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr>> OnElementSwapEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr>>(std::bind(&M3BoardGameplayController::OnElementSwapEnded, this, std::placeholders::_1));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr>> OnElementSwapEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr>>(M3Events::ON_ELEMENT_SWAP_ENDED, OnElementSwapEndedCallback);
	Subscribe(OnElementSwapEndedEvent);
}

M3BoardGameplayController::~M3BoardGameplayController() {
}

void M3BoardGameplayController::GeneratePotentialSwaps() {
	const auto SwapModel = M3SharedModel::GetInstance()->GetSubmodel<M3SwapModel>();
	SwapModel->GeneratePotentialSwaps();
}

bool M3BoardGameplayController::CanBeExecuted() const {
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	return BoardActionsAccumulationModel->HasActions();
}

void M3BoardGameplayController::Execute(float Deltatime) {
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	const auto& Actions = BoardActionsAccumulationModel->Entity->Get()->Actions->Get();
	while (!Actions->empty()) {
		const auto Action = Actions->front();
		Actions->pop_front();
		switch (Action.Action)
		{
			case EM3AccumulationAction::ON_ELEMENT_SWAP_ENDED: {
				M3BoardGameplayController::GeneratePotentialSwaps();
			}
				break;
			default:
				break;
			}
	}
}

void M3BoardGameplayController::OnGameStarted() {
	M3BoardGameplayController::GeneratePotentialSwaps();
}

void M3BoardGameplayController::OnElementSwapEnded(const M3ElementModel_SharedPtr& ElementModel) {
	ElementModel->SetState(EM3ElementState::IDLE);
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	BoardActionsAccumulationModel->PushAction(EM3AccumulationAction::ON_ELEMENT_SWAP_ENDED);
}
