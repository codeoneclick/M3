// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardStateModel.h"
#include "M3ElementModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"
#include "M3GlobalDispatcher.h"

M3BoardStateModel::M3BoardStateModel() {
	std::shared_ptr<M3AppEventModelProp_Callback> OnStateChangedCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&M3BoardStateModel::OnStateChanged, this, std::placeholders::_1, std::placeholders::_2));
	OnStateChangedEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnStateChangedCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnStateChangedEvent);
}

M3BoardStateModel::~M3BoardStateModel() {
	if (OnStateChangedEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnStateChangedEvent);
	}
}

void M3BoardStateModel::Init() {

}

void M3BoardStateModel::Serialize() {

}

void M3BoardStateModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3BoardStateModel::OnStateChanged(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
	const auto& ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	EM3ElementState State = ElementModel->GetState();
	for (auto& It : ElementsInState) {
		It.second.erase(ElementModel);
	}
	auto It = ElementsInState.find(State);
	if (It == ElementsInState.end()) {
		ElementsInState.emplace(std::make_pair(State, std::set<M3ElementModel_SharedPtr>()));
		It = ElementsInState.find(State);
	}
	It->second.insert(ElementModel);
}

void M3BoardStateModel::IncGameTurn() {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	if (BoardSettingsModel->GetIsTurnBased()) {
		Entity->Get()->Duration->Set(FMath::Min(Entity->Get()->Duration->Get() + 1, BoardSettingsModel->GetDuration()));
	}
	Entity->Get()->Turn->Set(Entity->Get()->Turn->Get() + 1);
}

void M3BoardStateModel::IncTimestamp() {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	Entity->Get()->Timestamp->Set(Entity->Get()->Timestamp->Get() + 1);
}

bool M3BoardStateModel::IsSomeElementInAction() const {
	bool Result = false;
	for (auto& It : ElementsInState) {
		if (It.first != EM3ElementState::IDLE && It.second.size() != 0) {
			Result = true;
			break;
		}
	}
	return Result;
}
