// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardGameplayController.h"
#include "M3AppEvent.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3CellModel.h"
#include "M3SwapModel.h"
#include "M3ChainModel.h"
#include "M3BoardActionsAccumulationModel.h"
#include "M3BoardStateModel.h"
#include "M3SharedModel.h"

M3BoardGameplayController::M3BoardGameplayController() {
	std::shared_ptr<M3AppEvent_Callback<int>> OnGameStartedCallback = std::make_shared<M3AppEvent_Callback<int>>(std::bind(&M3BoardGameplayController::OnGameStarted, this));
	std::shared_ptr<M3AppEvent<int>> OnGameStartedEvent = std::make_shared<M3AppEvent<int>>(M3Events::ON_GAME_STARTED, OnGameStartedCallback);
	Subscribe(OnGameStartedEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr, bool>> OnElementSwapEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr, bool>>(std::bind(&M3BoardGameplayController::OnElementSwapEnded, this, std::placeholders::_1, std::placeholders::_2));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr, bool>> OnElementSwapEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr, bool>>(M3Events::ON_ELEMENT_SWAP_ENDED, OnElementSwapEndedCallback);
	Subscribe(OnElementSwapEndedEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr>> OnElementMatchEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr>>(std::bind(&M3BoardGameplayController::OnElementMatchEnded, this, std::placeholders::_1));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr>> OnElementMatchEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr>>(M3Events::ON_ELEMENT_MATCH_ENDED, OnElementMatchEndedCallback);
	Subscribe(OnElementMatchEndedEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr>> OnElementDropEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr>>(std::bind(&M3BoardGameplayController::OnElementDropEnded, this, std::placeholders::_1));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr>> OnElementDropEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr>>(M3Events::ON_ELEMENT_DROP_ENDED, OnElementDropEndedCallback);
	Subscribe(OnElementDropEndedEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr>> OnElementSpawnEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr>>(std::bind(&M3BoardGameplayController::OnElementSpawnEnded, this, std::placeholders::_1));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr>> OnElementSpawnEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr>>(M3Events::ON_ELEMENT_SPAWN_ENDED, OnElementSpawnEndedCallback);
	Subscribe(OnElementSpawnEndedEvent);
}

M3BoardGameplayController::~M3BoardGameplayController() {
}

void M3BoardGameplayController::GeneratePotentialSwaps() {
	const auto SwapModel = M3SharedModel::GetInstance()->GetSubmodel<M3SwapModel>();
	SwapModel->GeneratePotentialSwaps();
}

void M3BoardGameplayController::DetectMatches() {
	const auto BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	const auto ChainModel = M3SharedModel::GetInstance()->GetSubmodel<M3ChainModel>();

	ChainModel->DetectHorizontalMatches(BoardModel);
	ChainModel->DetectVerticalMatches(BoardModel);
}

void M3BoardGameplayController::CreateHoles() {
	const auto BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	BoardModel->RemoveMatched();
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
		switch (Action.Action) {
			case EM3AccumulationAction::ON_ELEMENT_SWAP_ENDED:
			case EM3AccumulationAction::ON_ELEMENT_DROP_ENDED:
			case EM3AccumulationAction::ON_ELEMENT_SPAWN_ENDED:
				M3BoardGameplayController::GeneratePotentialSwaps();
				M3BoardGameplayController::DetectMatches();
				if (Action.Action == EM3AccumulationAction::ON_ELEMENT_SWAP_ENDED) {
					const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
					BoardStateModel->IncGameTurn();
				}
				break;
			case EM3AccumulationAction::ON_ELEMENT_MATCH_ENDED:
				M3BoardGameplayController::CreateHoles();
				M3BoardGameplayController::GeneratePotentialSwaps();
				break;

			default:
				break;
		}
	}
}

void M3BoardGameplayController::OnGameStarted() {
	M3BoardGameplayController::GeneratePotentialSwaps();
	M3BoardGameplayController::DetectMatches();
}

void M3BoardGameplayController::OnElementSwapEnded(const M3ElementModel_SharedPtr& ElementModel, bool IsPossibleToSwap) {
	ElementModel->SetState(EM3ElementState::IDLE);
	if (IsPossibleToSwap) {
		const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
		BoardActionsAccumulationModel->PushAction(EM3AccumulationAction::ON_ELEMENT_SWAP_ENDED);
	}
}

void M3BoardGameplayController::OnElementMatchEnded(const M3ElementModel_SharedPtr& ElementModel) {
	ElementModel->SetState(EM3ElementState::IDLE);
	ElementModel->SetState(EM3ElementState::REMOVING);

	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	BoardActionsAccumulationModel->PushAction(EM3AccumulationAction::ON_ELEMENT_MATCH_ENDED);
}

void M3BoardGameplayController::OnElementDropEnded(const M3ElementModel_SharedPtr& ElementModel) {
	ElementModel->SetState(EM3ElementState::IDLE);
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	BoardActionsAccumulationModel->PushAction(EM3AccumulationAction::ON_ELEMENT_DROP_ENDED);
}

void M3BoardGameplayController::OnElementSpawnEnded(const M3ElementModel_SharedPtr& ElementModel) {
	ElementModel->SetState(EM3ElementState::IDLE);
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	BoardActionsAccumulationModel->PushAction(EM3AccumulationAction::ON_ELEMENT_SPAWN_ENDED);
}
