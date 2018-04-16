// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BlockerModel.h"
#include "M3BoardStateModel.h"
#include "M3SharedModel.h"

M3BlockerModel::M3BlockerModel() {
}

M3BlockerModel::~M3BlockerModel() {
}

void M3BlockerModel::Init() {
	std::shared_ptr<M3BlockerModel> Instance = std::static_pointer_cast<M3BlockerModel>(shared_from_this());
	AddComponent(std::make_shared<M3ElementModelBlockerComponent>(std::static_pointer_cast<M3ElementModelBlockerComponent_INTERFACE>(Instance)));
}

void M3BlockerModel::Serialize() {

}

void M3BlockerModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	AM3CellAppointmentScheme* Appointment = Cast<AM3CellAppointmentScheme>(Scheme);
	Entity->Get()->Id->Set(Appointment->Id);

	switch (Appointment->Id)
	{
	case EM3ElementId::BLOCKER_BOX1X:
		Entity->Get()->Density->Set(1);
		break;
	case EM3ElementId::BLOCKER_BOX2X:
		Entity->Get()->Density->Set(2);
		break;
	case EM3ElementId::BLOCKER_BOX3X:
		Entity->Get()->Density->Set(3);
		break;
	case EM3ElementId::BLOCKER_ICE1X:
		Entity->Get()->Density->Set(1);
		break;
	case EM3ElementId::BLOCKER_ICE2X:
		Entity->Get()->Density->Set(2);
		break;
	case EM3ElementId::BLOCKER_WIRE1X:
		Entity->Get()->Density->Set(1);
		break;
	case EM3ElementId::BLOCKER_WIRE2X:
		Entity->Get()->Density->Set(2);
		break;
	default:
		break;
	}
}

void M3BlockerModel::Reset() {
	M3Model::Reset();

	Entity->Get()->Id->Set(EM3ElementId::UNKNOWN);
}

void M3BlockerModel::DecDensity() {
	const auto& BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	if (Entity->Get()->AffectedOnTurn->Get() != BoardStateModel->GetTurn()) {
		Entity->Get()->Density->Set(FMath::Max(Entity->Get()->Density->Get() - 1, 0));
		Entity->Get()->AffectedOnTurn->Set(BoardStateModel->GetTurn());
	}
}

bool M3BlockerModel::CanDrop() {
	bool Result = false;
	if (GetId() == EM3ElementId::BLOCKER_ICE1X ||
		GetId() == EM3ElementId::BLOCKER_ICE2X) {
		Result = true;
	}
	return Result;
}

bool M3BlockerModel::CanSwap() {
	return false;
}

bool M3BlockerModel::CanMatch() {
	bool Result = true;
	if (GetId() == EM3ElementId::BLOCKER_BOX1X ||
		GetId() == EM3ElementId::BLOCKER_BOX2X ||
		GetId() == EM3ElementId::BLOCKER_BOX3X) {
		Result = false;
	}
	return Result;
}

bool M3BlockerModel::CanRemove() {
	return Entity->Get()->Density->Get() == 0;
}
