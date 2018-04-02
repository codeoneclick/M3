// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementModel.h"
#include "M3RegularelementModel.h"

M3ElementModelColorComponent::M3ElementModelColorComponent(const std::shared_ptr<M3ElementModelColorComponent_INTERFACE>& _Owner) {
	Owner = _Owner;
}

EM3ElementColor M3ElementModelColorComponent::GetColor() {
	return Owner->GetColor();
}

M3ElementModel::M3ElementModel() {
}

M3ElementModel::~M3ElementModel() {
}

void M3ElementModel::Init() {
	SUBSCRIBE_PROP(M3ElementEntity, State)
}

void M3ElementModel::Serialize() {

}

void M3ElementModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3ElementModel::Reset() {
	M3Model::Reset();

	Entity->Get()->State->Set(EM3ElementState::IDLE);
}

EM3ElementColor M3ElementModel::GetColor() {
	EM3ElementColor Result = EM3ElementColor::NONE;

	for (const auto Submodel : Submodels) {
		if (Submodel && Submodel->GetComponent<M3ElementModelColorComponent>()) {
			Result = Submodel->GetComponent<M3ElementModelColorComponent>()->GetColor();
			break;
		}
	}
	return Result;
}

void M3ElementModel::SetState(EM3ElementState State) {
	if (State != EM3ElementState::IDLE) {
		assert(Entity->Get()->State->Get() == EM3ElementState::IDLE);
		if (Entity->Get()->State->Get() != EM3ElementState::IDLE) {
			UE_LOG(LogTemp, Error, TEXT("Wrong element state!"));
		}
	}
	switch (State) {
	case EM3ElementState::SPAWNING:
	case EM3ElementState::SWAPPING:
	case EM3ElementState::DROPPING: {
		//
	}
		break;

	default: {
		// NOTHING
	}
		break;
	}
	Entity->Get()->State->Set(State);
}

EM3ElementState M3ElementModel::GetState() const {
	return Entity->Get()->State->Get();
}

bool M3ElementModel::IsInState(EM3ElementState State) const {
	return GetState() == State;
}

bool M3ElementModel::IsInIdle() const {
	return IsInState(EM3ElementState::IDLE);
}

bool M3ElementModel::CanMatch() const {
	return IsInState(EM3ElementState::IDLE);
}

bool M3ElementModel::CanDrop() const {
	return IsInState(EM3ElementState::IDLE);
}

bool M3ElementModel::IsDropBlocked() const {
	return false;
}
