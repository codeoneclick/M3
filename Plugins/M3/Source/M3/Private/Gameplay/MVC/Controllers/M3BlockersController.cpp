// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BlockersController.h"
#include "M3ElementModel.h"
#include "M3BlockerModel.h"
#include "M3AppEvent.h"

M3BlockersController::M3BlockersController() {
	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr>> OnElementMatchEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr>>(std::bind(&M3BlockersController::OnElementMatchEnded, this, std::placeholders::_1));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr>> OnElementMatchEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr>>(M3Events::ON_ELEMENT_MATCH_ENDED, OnElementMatchEndedCallback);
	Subscribe(OnElementMatchEndedEvent);
}

M3BlockersController::~M3BlockersController() {
}

bool M3BlockersController::CanBeExecuted() const {
	return !BlockersToRemove.empty();
}

void M3BlockersController::Execute(float Deltatime) {
	for (const auto& BlockerModel : BlockersToRemove) {
		const auto& ElementModel = BlockerModel->GetParent<M3ElementModel>();
		ElementModel->RemoveSubmodel(BlockerModel);
		M3BlockerModel::Destruct(BlockerModel);
	}
	BlockersToRemove.clear();
}

void M3BlockersController::OnBlockerAffected(const M3ElementModel_SharedPtr& ElementModel, const M3BlockerModel_SharedPtr& BlockerModel) {
	BlockerModel->DecDensity();
	if (BlockerModel->GetDensity() == 0) {
		BlockersToRemove.insert(BlockerModel);
	}
	else {
		switch (BlockerModel->GetId()) {
		case EM3ElementId::BLOCKER_BOX3X:
			BlockerModel->Entity->Get()->Id->Set(EM3ElementId::BLOCKER_BOX2X);
			break;
		case EM3ElementId::BLOCKER_BOX2X:
			BlockerModel->Entity->Get()->Id->Set(EM3ElementId::BLOCKER_BOX1X);
			break;
		case EM3ElementId::BLOCKER_ICE2X:
			BlockerModel->Entity->Get()->Id->Set(EM3ElementId::BLOCKER_ICE1X);
			break;
		case EM3ElementId::BLOCKER_WIRE2X:
			BlockerModel->Entity->Get()->Id->Set(EM3ElementId::BLOCKER_WIRE1X);
			break;
		default:
			break;
		}
	}
}

void M3BlockersController::OnElementMatchEnded(const M3ElementModel_SharedPtr& _ElementModel) {
	const auto& BlockerModel = _ElementModel->GetSubmodel<M3BlockerModel>();
	if (BlockerModel) {
		OnBlockerAffected(_ElementModel, BlockerModel);
	} else {
		const auto& Blockers = M3BlockerModel::Container()->Get();
		for (auto It = Blockers->begin(); It != Blockers->end(); ++It) {
			const auto& BlockerModel = std::static_pointer_cast<M3BlockerModel>((*It));
			if (BlockerModel->GetId() == EM3ElementId::BLOCKER_BOX3X ||
				BlockerModel->GetId() == EM3ElementId::BLOCKER_BOX2X ||
				BlockerModel->GetId() == EM3ElementId::BLOCKER_BOX1X) {
				const auto& ElementModel = (*It)->GetParent<M3ElementModel>();
				if (M3ElementModel::IsNeighbours(_ElementModel, ElementModel)) {
					OnBlockerAffected(_ElementModel, BlockerModel);
				}
			}
		}
	}
}
