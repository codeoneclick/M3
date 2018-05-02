// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementModel.h"
#include "M3BoardStateModel.h"
#include "M3RegularElementModel.h"
#include "M3SuperElementModel.h"
#include "M3SharedModel.h"
#include "M3CellModel.h"

M3ElementModelColorComponent::M3ElementModelColorComponent(const std::shared_ptr<M3ElementModelColorComponent_INTERFACE>& _Owner) {
	Owner = _Owner;
}

EM3ElementColor M3ElementModelColorComponent::GetColor() {
	return Owner->GetColor();
}

M3ElementModelBlockerComponent::M3ElementModelBlockerComponent(const std::shared_ptr<M3ElementModelBlockerComponent_INTERFACE>& _Owner) {
	Owner = _Owner;
}

bool M3ElementModelBlockerComponent::CanDrop() const {
	return Owner->CanDrop();
}

bool M3ElementModelBlockerComponent::CanSwap() const {
	return Owner->CanSwap();
}

bool M3ElementModelBlockerComponent::CanMatch() const {
	return Owner->CanMatch();
}

bool M3ElementModelBlockerComponent::CanRemove() const {
	return Owner->CanRemove();
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
	Entity->Get()->Timestamp->Set(0);
}

EM3ElementColor M3ElementModel::GetColor() {
	EM3ElementColor Result = EM3ElementColor::NONE;

	for (const auto Submodel : Submodels) {
		if (Submodel.second && Submodel.second->GetComponent<M3ElementModelColorComponent>()) {
			Result = Submodel.second->GetComponent<M3ElementModelColorComponent>()->GetColor();
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
		const auto& BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
		Entity->Get()->Timestamp->Set(BoardStateModel->GetTimestamp());
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
	bool Result = IsInState(EM3ElementState::IDLE);
	if (Result) {
		for (const auto Submodel : Submodels) {
			if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
				Result = Submodel.second->GetComponent<M3ElementModelBlockerComponent>()->CanMatch();
				break;
			}
		}
	}
	return Result;
}

bool M3ElementModel::CanDrop() const {
	bool Result = IsInState(EM3ElementState::IDLE);
	if (Result) {
		for (const auto Submodel : Submodels) {
			if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
				Result = Submodel.second->GetComponent<M3ElementModelBlockerComponent>()->CanDrop();
				break;
			}
		}
	}
	return Result;
}

bool M3ElementModel::CanSwap() const {
	bool Result = IsInState(EM3ElementState::IDLE);
	if (Result) {
		for (const auto Submodel : Submodels) {
			if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
				Result = Submodel.second->GetComponent<M3ElementModelBlockerComponent>()->CanSwap();
				break;
			}
		}
	}
	return Result;
}

bool M3ElementModel::CanRemove() const {
	bool Result = true;
	for (const auto Submodel : Submodels) {
		if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
			Result = Submodel.second->GetComponent<M3ElementModelBlockerComponent>()->CanRemove();
			break;
		}
	}
	return Result;
}

bool M3ElementModel::IsDropBlocked() const {
	bool Result = false;
	for (const auto Submodel : Submodels) {
		if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
			Result = !Submodel.second->GetComponent<M3ElementModelBlockerComponent>()->CanDrop();
			break;
		}
	}
	return Result;
}

bool M3ElementModel::IsMatchBlocked() const {
	bool Result = false;
	for (const auto Submodel : Submodels) {
		if (Submodel.second && Submodel.second->GetComponent<M3ElementModelBlockerComponent>()) {
			Result = true;
			break;
		}
	}
	return Result;
}

bool M3ElementModel::IsRegular() const {
	return GetSubmodel<M3RegularElementModel>() != nullptr;
}

bool M3ElementModel::IsSuper() const {
	return GetSubmodel<M3SuperElementModel>() != nullptr;
}

bool M3ElementModel::IsNeighbours(const M3ElementModel_SharedPtr& CurrentElementModel, const M3ElementModel_SharedPtr& NeighbourElementModel) {
	bool Result = false;

	const auto& CurrentCellModel = CurrentElementModel->GetParent<M3CellModel>();
	const auto& NeighbourCellModel = NeighbourElementModel->GetParent<M3CellModel>();

	Result = M3CellModel::IsNeighbours(CurrentCellModel, NeighbourCellModel);

	return Result;
}
