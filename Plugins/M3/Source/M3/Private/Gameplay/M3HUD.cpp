// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3HUD.h"
#include "M3BoardStateModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"
#include "M3AppEvent.h"
#include "M3KVSlot.h"
#include "M3GoalsModel.h"

const std::string k_ON_DURATION_CHANGED = "ON_DURATION_CHANGED";
const std::string k_ON_SCORES_CHANGED = "ON_SCORES_CHANGED";

AM3HUD::AM3HUD() {
	PrimaryActorTick.bCanEverTick = true;
	std::shared_ptr<M3AppEvent_Callback<int>> OnGameStartedCallback = std::make_shared<M3AppEvent_Callback<int>>(std::bind(&AM3HUD::OnGameStarted, this));
	OnGameStartedEvent = std::make_shared<M3AppEvent<int>>(M3Events::ON_GAME_STARTED, OnGameStartedCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnGameStartedEvent);

	std::shared_ptr<M3AppEventModelProp_Callback> OnGoalQuantityChangedCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&AM3HUD::OnGoalQuantityChanged, this, std::placeholders::_1, std::placeholders::_2));
	OnGoalQuantityChangedEvent = std::make_shared<M3AppEventModelProp>(M3GoalModel::ClassGuid(), M3GoalEntity::PROPERTY_ID_Quantity(), OnGoalQuantityChangedCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnGoalQuantityChangedEvent);
}

AM3HUD::~AM3HUD() {
	if (OnGameStartedEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnGameStartedEvent);
	}
	if (OnGoalQuantityChangedEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnGoalQuantityChangedEvent);
	}
	for (const auto& Slot : Slots) {
		Slot.second->DetachAll();
	}
	Slots.clear();
}

void AM3HUD::BeginPlay() {
	Super::BeginPlay();
}

void AM3HUD::OnGameStarted() {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();

	std::shared_ptr<M3KVSlot<int>> OnDurationChangedSlot = std::make_shared<M3KVSlot<int>>(BoardStateModel->Entity->Get()->Duration);
	Slots[k_ON_DURATION_CHANGED] = OnDurationChangedSlot;
	OnDurationChangedSlot->Attach([=](int _Duration) {
		OnDurationChanged(BoardStateModel->GetDuration(), BoardSettingsModel->GetDuration(), BoardSettingsModel->GetIsTurnBased());
	});
	OnDurationChanged(BoardStateModel->GetDuration(), BoardSettingsModel->GetDuration(), BoardSettingsModel->GetIsTurnBased());

	std::shared_ptr<M3KVSlot<int>> OnScoresChangedSlot = std::make_shared<M3KVSlot<int>>(BoardStateModel->Entity->Get()->Scores);
	Slots[k_ON_SCORES_CHANGED] = OnScoresChangedSlot;
	OnScoresChangedSlot->Attach([=](int _Scores) {
		OnScoresChanged(BoardStateModel->GetScores(), BoardSettingsModel->GetStar1Scores(), BoardSettingsModel->GetStar2Scores(), BoardSettingsModel->GetStar3Scores());
	});
	OnScoresChanged(BoardStateModel->GetScores(), BoardSettingsModel->GetStar1Scores(), BoardSettingsModel->GetStar2Scores(), BoardSettingsModel->GetStar3Scores());

	const auto GoalsModel = M3SharedModel::GetInstance()->GetSubmodel<M3GoalsModel>();
	for (size_t i = 0; i < GoalsModel->GetGoals()->size(); ++i) {
		OnGoalChanged(GoalsModel->GetGoals()->data()[i]->GetId(), GoalsModel->GetGoals()->data()[i]->GetIndex(), GoalsModel->GetGoals()->data()[i]->GetQuantity(), GoalsModel->GetGoals()->data()[i]->GetMaxQuantity());
	}
}

void AM3HUD::OnGoalQuantityChanged(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
	const auto GoalModel = std::static_pointer_cast<M3GoalModel>(Model);
	OnGoalChanged(GoalModel->GetId(), GoalModel->GetIndex(), GoalModel->GetQuantity(), GoalModel->GetMaxQuantity());
}


