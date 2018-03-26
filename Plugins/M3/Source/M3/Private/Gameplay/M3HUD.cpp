// Fill out your copyright notice in the Description page of Project Settings.

#include "M3HUD.h"
#include "M3BoardStateModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"
#include "M3AppEvent.h"
#include "M3KVSlot.h"

const std::string k_ON_DURATION_CHANGED = "ON_DURATION_CHANGED";
const std::string k_ON_SCORES_CHANGED = "ON_SCORES_CHANGED";

AM3HUD::AM3HUD() {
	PrimaryActorTick.bCanEverTick = true;
	std::shared_ptr<M3AppEvent_Callback<int>> OnGameStartedCallback = std::make_shared<M3AppEvent_Callback<int>>(std::bind(&AM3HUD::OnGameStarted, this));
	OnGameStartedEvent = std::make_shared<M3AppEvent<int>>(M3Events::ON_GAME_STARTED, OnGameStartedCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnGameStartedEvent);
}

AM3HUD::~AM3HUD() {
	if (OnGameStartedEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnGameStartedEvent);
	}
	for (const auto& Slot : Slots) {
		Slot.second->DetachAll();
	}
	Slots.clear();
}

void AM3HUD::BeginPlay() {
	Super::BeginPlay();

	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	OnDurationChanged(BoardStateModel->GetDuration(), BoardSettingsModel->GetDuration(), BoardSettingsModel->GetIsTurnBased());
	OnScoresChanged(BoardStateModel->GetScores(), BoardSettingsModel->GetStar1Scores(), BoardSettingsModel->GetStar2Scores(), BoardSettingsModel->GetStar3Scores());
}

void AM3HUD::OnGameStarted() {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	const auto BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();

	std::shared_ptr<M3KVSlot<int>> OnDurationChangedSlot = std::make_shared<M3KVSlot<int>>(BoardStateModel->Entity->Get()->Duration);
	Slots[k_ON_DURATION_CHANGED] = OnDurationChangedSlot;
	OnDurationChangedSlot->Attach([=](int _Duration) {
		OnDurationChanged(BoardStateModel->GetDuration(), BoardSettingsModel->GetDuration(), BoardSettingsModel->GetIsTurnBased());
	});

	std::shared_ptr<M3KVSlot<int>> OnScoresChangedSlot = std::make_shared<M3KVSlot<int>>(BoardStateModel->Entity->Get()->Scores);
	Slots[k_ON_SCORES_CHANGED] = OnScoresChangedSlot;
	OnScoresChangedSlot->Attach([=](int _Scores) {
		OnScoresChanged(BoardStateModel->GetScores(), BoardSettingsModel->GetStar1Scores(), BoardSettingsModel->GetStar2Scores(), BoardSettingsModel->GetStar3Scores());
	});
}


