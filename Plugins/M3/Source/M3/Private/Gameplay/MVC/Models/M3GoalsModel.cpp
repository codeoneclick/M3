// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3GoalsModel.h"

M3GoalModel::M3GoalModel() {

}

M3GoalModel::~M3GoalModel() {

}

void M3GoalModel::Serialize() {

}

void M3GoalModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	const auto GoalScheme = static_cast<AM3GoalScheme*>(Scheme);
	Entity->Get()->Id->Set(GoalScheme->Id);
	Entity->Get()->MaxQuantity->Set(GoalScheme->Quantity);
}

void M3GoalModel::Init() {
	SUBSCRIBE_PROP(M3GoalEntity, Quantity)
}

void M3GoalModel::IncQuantity() {
	Entity->Get()->Quantity->Set(FMath::Min(Entity->Get()->Quantity->Get() + 1, Entity->Get()->MaxQuantity->Get()));
}

bool M3GoalModel::IsDone() const {
	return GetQuantity() >= GetMaxQuantity();
}

M3GoalsModel::M3GoalsModel() {

}

M3GoalsModel::~M3GoalsModel() {

}

void M3GoalsModel::Serialize() {

}

void M3GoalsModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	const auto BoardScheme = static_cast<AM3BoardScheme*>(Scheme);
	for (const auto Goal : BoardScheme->Goals) {
		if (Goal->Enabled && Goal->Quantity != 0) {
			const auto GoalModel = M3GoalModel::Construct<M3GoalModel>();
			GoalModel->Deserialize(Goal);
			GoalModel->Entity->Get()->Index->Set(Entity->Get()->Goals->Get()->size());
			Entity->Get()->Goals->Get()->push_back(GoalModel);
		}
	}
}

void M3GoalsModel::Init() {

}
