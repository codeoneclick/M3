// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardActionsAccumController.h"
#include "M3BoardActionsAccumulationModel.h"
#include "M3SharedModel.h"

M3BoardActionsAccumController::M3BoardActionsAccumController() {
}

M3BoardActionsAccumController::~M3BoardActionsAccumController() {
}

bool M3BoardActionsAccumController::CanBeExecuted() const {
	return true;
}

void M3BoardActionsAccumController::Execute(float Deltatime) {
	const auto& BoardActionsAccumulationModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardActionsAccumulationModel>();
	int Revision = BoardActionsAccumulationModel->Entity->Get()->Revision->Get();
	++Revision;
	BoardActionsAccumulationModel->Entity->Get()->Revision->Set(Revision);
}
