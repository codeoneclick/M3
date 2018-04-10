// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperElementsController.h"
#include "M3ElementModel.h"
#include "M3SuperElementModel.h"
#include "M3AppEvent.h"
#include "M3CellModel.h"
#include "M3BoardModel.h"
#include "M3SharedModel.h"

M3SuperElementsController::M3SuperElementsController() {
	std::shared_ptr<M3AppEventModelProp_Callback> OnMatchingsCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&M3SuperElementsController::OnMatching, this, std::placeholders::_1, std::placeholders::_2));
	std::shared_ptr<M3AppEventModelProp> OnMatchingEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnMatchingsCallback);
	Subscribe(OnMatchingEvent);

	std::shared_ptr<M3AppEvent_Callback<M3ElementModel_SharedPtr, bool>> OnElementSwapEndedCallback = std::make_shared<M3AppEvent_Callback<M3ElementModel_SharedPtr, bool>>(std::bind(&M3SuperElementsController::OnSwapEnded, this, std::placeholders::_1, std::placeholders::_2));
	std::shared_ptr<M3AppEvent<M3ElementModel_SharedPtr, bool>> OnElementSwapEndedEvent = std::make_shared<M3AppEvent<M3ElementModel_SharedPtr, bool>>(M3Events::ON_ELEMENT_SWAP_ENDED, OnElementSwapEndedCallback);
	Subscribe(OnElementSwapEndedEvent);
}

M3SuperElementsController::~M3SuperElementsController() {
}

bool M3SuperElementsController::CanBeExecuted() const {
	return !SuperElementsToExecute.empty();
}

void M3SuperElementsController::Execute(float Deltatime) {
	while (!SuperElementsToExecute.empty()) {
		const auto SuperElementModel = SuperElementsToExecute.front();
		SuperElementsToExecute.pop();
		const auto ElementModel = SuperElementModel->GetParent<M3ElementModel>();
		ensure(ElementModel != nullptr);
		const auto CellModel = ElementModel->GetParent<M3CellModel>();
		ensure(CellModel != nullptr);
		OnExplosion(CellModel->GetCol(), CellModel->GetRow(), M3SuperElementModel::GetExplosionRadius(SuperElementModel->GetId()));
	}
}

void M3SuperElementsController::OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
	const auto ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	if (ElementModel->IsSuper() && ElementModel->IsInState(EM3ElementState::MATCHING)) {
		const auto SuperElementModel = ElementModel->GetSubmodel<M3SuperElementModel>();
		SuperElementsToExecute.push(SuperElementModel);
	}
}

void M3SuperElementsController::OnSwapEnded(const M3ElementModel_SharedPtr& ElementModel, bool IsPossibleToSwap) {
	if (ElementModel->IsSuper()) {
		ElementModel->SetState(EM3ElementState::MATCHING);
	}
}

void M3SuperElementsController::OnExplosion(int Col, int Row, int Radius) {
	const auto& BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
	M3ElementModel_SharedPtr ElementModel = nullptr;
	for (int i = Col - Radius; i <= Col + Radius; ++i) {
		for (int j = Row - Radius; j <= Row + Radius; ++j) {
			if ((i == Col - Radius && j == Row - Radius) ||
				(i == Col - Radius && j == Row + Radius) ||
				(i == Col + Radius && j == Row - Radius) ||
				(i == Col + Radius && j == Row + Radius)) {
				continue;
			}
			ElementModel = BoardModel->GetElement(i, j);
			if (ElementModel) {
				bool IsSuperElementsChain = false;
				if (i != Col || j != Row) {
					if (ElementModel->IsSuper() && !ElementModel->IsInState(EM3ElementState::MATCHING)) {
						IsSuperElementsChain = true;
						const auto SuperElementsModel = ElementModel->GetSubmodel<M3SuperElementModel>();
						SuperElementsToExecute.push(SuperElementsModel);
					}
				}

				if (!IsSuperElementsChain && !ElementModel->IsInState(EM3ElementState::MATCHING)) {
					ElementModel->SetState(EM3ElementState::IDLE);
					ElementModel->SetState(EM3ElementState::MATCHING);
				}
			}
		}
	}
}
