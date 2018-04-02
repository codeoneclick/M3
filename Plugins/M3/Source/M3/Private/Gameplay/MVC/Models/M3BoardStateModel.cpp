// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardStateModel.h"
#include "M3ElementModel.h"
#include "M3BoardSettingsModel.h"
#include "M3SharedModel.h"

M3BoardStateModel::M3BoardStateModel() {
}

M3BoardStateModel::~M3BoardStateModel() {
}

void M3BoardStateModel::Init() {

}

void M3BoardStateModel::Serialize() {

}

void M3BoardStateModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3BoardStateModel::IncGameTurn() {
	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();
	if (BoardSettingsModel->GetIsTurnBased()) {
		Entity->Get()->Duration->Set(FMath::Min(Entity->Get()->Duration->Get() + 1, BoardSettingsModel->GetDuration()));
	}
	Entity->Get()->Turn->Set(Entity->Get()->Turn->Get() + 1);
}
