// Copyright serhii serhiiv 2017. All rights reserved.

#include "M3GoalsController.h"
#include "M3AppEvent.h"
#include "M3ElementModel.h"
#include "M3GoalsModel.h"
#include "M3SharedModel.h"

M3GoalsController::M3GoalsController() {
	std::shared_ptr<M3AppEventModelProp_Callback> OnMatchingsCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&M3GoalsController::OnMatching, this, std::placeholders::_1, std::placeholders::_2));
	std::shared_ptr<M3AppEventModelProp> OnMatchingEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnMatchingsCallback);
	Subscribe(OnMatchingEvent);
}

M3GoalsController::~M3GoalsController() {

}

bool M3GoalsController::CanBeExecuted() const {
	return false;
}

void M3GoalsController::Execute(float Deltatime) {

}

void M3GoalsController::OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
	const auto GoalsModel = M3SharedModel::GetInstance()->GetSubmodel<M3GoalsModel>();
	const auto ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	for (int i = 0; i < GoalsModel->GetGoals()->size(); ++i) {
		if (static_cast<int>(GoalsModel->GetGoals()->data()[i].Id) == ElementModel->GetElementId()) {
			if (GoalsModel->GetGoals()->data()[i].Quantity < GoalsModel->GetGoals()->data()[i].MaxQuantity) {
				GoalsModel->GetGoals()->data()[i].Quantity++;
			} else {
				UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
			}
		}
	}
}
