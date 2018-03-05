// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardActionsAccumulationModel.h"

M3BoardActionsAccumulationModel::M3BoardActionsAccumulationModel() {
}

M3BoardActionsAccumulationModel::~M3BoardActionsAccumulationModel() {
}

void M3BoardActionsAccumulationModel::Init() {

}

void M3BoardActionsAccumulationModel::Serialize() {

}

void M3BoardActionsAccumulationModel::Deserialize(UM3Scheme_INTERFACE* Scheme) {

}

void M3BoardActionsAccumulationModel::PushAction(EM3AccumulationAction _Action) {
	const auto& Actions = Entity->Get()->Actions->Get();
	int Revision = Entity->Get()->Revision->Get();
	const auto& It = std::find_if(Actions->begin(), Actions->end(), [_Action, Revision](const M3AccumulationAction& Action) {
		return Action.Action == _Action && Action.Revision == Revision;
	});
	if (It == Actions->end()) {
		M3AccumulationAction Action;
		Action.Action = _Action;
		Action.Revision = Revision;
		Actions->push_back(Action);
	}
}

bool M3BoardActionsAccumulationModel::HasActions() const {
	const auto& Actions = Entity->Get()->Actions->Get();
	return Actions->size() != 0;
}
