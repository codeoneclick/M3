// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementModel.h"

M3ElementModel::M3ElementModel()
{
}

M3ElementModel::~M3ElementModel()
{
}

void M3ElementModel::Init() {
	SUBSCRIBE_PROP(M3ElementEntity, ElementId)
	SUBSCRIBE_PROP(M3ElementEntity, State)
}

void M3ElementModel::Serialize() {

}

void M3ElementModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3ElementModel::Reset() {
	Entity->Get()->ElementId->Set(-1);
	Entity->Get()->State->Set(EM3ElementState::IDLE);
}

int M3ElementModel::GetElementId() const {
	return Entity->Get()->ElementId->Get();
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
