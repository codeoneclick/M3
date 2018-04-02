// Copyright serhii serhiiv 2018 All rights reserved.

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
	const auto ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	if (ElementModel->IsInState(EM3ElementState::MATCHING)) {
		const auto GoalsModel = M3SharedModel::GetInstance()->GetSubmodel<M3GoalsModel>();
		for (size_t i = 0; i < GoalsModel->GetGoals()->size(); ++i) {
			switch (GoalsModel->GetGoals()->data()[i]->GetId()) {
			case EM3GoalId::COLLECT_RED_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::RED) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			case EM3GoalId::COLLECT_GREEN_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::GREEN) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			case EM3GoalId::COLLECT_BLUE_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::BLUE) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			case EM3GoalId::COLLECT_YELLOW_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::YELLOW) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			case EM3GoalId::COLLECT_ORANGE_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::ORANGE) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			case EM3GoalId::COLLECT_PURPLE_ELEMENTS:
				if (ElementModel->GetColor() == EM3ElementColor::PURPLE) {
					if (!GoalsModel->GetGoals()->data()[i]->IsDone()) {
						GoalsModel->GetGoals()->data()[i]->IncQuantity();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Goal is done!"));
					}
				}
				break;

			default:
				break;
			}
		}
	}
}
