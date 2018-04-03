// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementsMatchController.h"
#include "M3ChainModel.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3RegularelementModel.h"
#include "M3SuperelementModel.h"
#include "M3BoardSettingsModel.h"
#include "M3BoardStateModel.h"
#include "M3SpawnModel.h"
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
	const auto BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	const auto SpawnModel = M3SharedModel::GetInstance()->GetSubmodel<M3SpawnModel>();

	ChainModel->ValidateSuperelementChains();

	while (ChainModel->IsSuperelementChainsExist()) {
		auto SuperelementChain = ChainModel->PopSuperelementChain();
		int MaxTimestamp = -1;
		M3ElementModel_SharedPtr ElementWithMaxTimestamp = nullptr;
		for (const auto Element : SuperelementChain->Elements) {
			ensure(Element->CanMatch());
			if (Element->GetTimestamp() > MaxTimestamp) {
				MaxTimestamp = Element->GetTimestamp();
				ElementWithMaxTimestamp = Element;
			}
		}
		ensure(ElementWithMaxTimestamp != nullptr);
		if (ElementWithMaxTimestamp) {
			ChainModel->ValidateElementChains(ElementWithMaxTimestamp);
			
			const auto CellModel = ElementWithMaxTimestamp->GetParent<M3CellModel>();
			BoardModel->CreateHole(CellModel->GetCol(), CellModel->GetRow());
			SpawnModel->PushSuperElementSpawner(CellModel);
			
			UE_LOG(LogTemp, Warning, TEXT("Superelement model added at cell %d %d"), CellModel->GetCol(), CellModel->GetRow());
		}
	}

	while (ChainModel->IsChainsExist()) {
		const auto Chain = ChainModel->PopChain();
		for (const auto Element : Chain->Elements) {
			ensure(Element->GetSubmodel<M3SuperelementModel>() == nullptr);
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
