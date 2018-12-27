// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SpawnModel.h"

M3SuperElementSpawner::M3SuperElementSpawner(const M3CellModel_SharedPtr& _Cell, int _Length) {
	Cell = _Cell;
	Length = _Length;
}

M3CellModel_SharedPtr M3SuperElementSpawner::GetCell() const {
	return Cell;
}

int M3SuperElementSpawner::GetLength() const {
	return Length;
}

M3SpawnModel::M3SpawnModel() {
}

M3SpawnModel::~M3SpawnModel() {
}

void M3SpawnModel::Init() {

}

void M3SpawnModel::Serialize() {

}

void M3SpawnModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

bool M3SpawnModel::IsSuperElementSpawnersExist() const {
	return Entity->Get()->SuperElementSpawners->Get()->size() != 0;
}

void M3SpawnModel::PushSuperElementSpawner(const M3CellModel_SharedPtr& Cell, int Length) {
	const auto SuperElementSpawner = std::make_shared<M3SuperElementSpawner>(Cell, Length);
	Entity->Get()->SuperElementSpawners->Get()->push_back(SuperElementSpawner);
}

M3SuperElementSpawner_SharedPtr M3SpawnModel::PopSuperElementSpawner() {
	M3SuperElementSpawner_SharedPtr Result = nullptr;
	if (IsSuperElementSpawnersExist()) {
		Result = Entity->Get()->SuperElementSpawners->Get()->back();
		Entity->Get()->SuperElementSpawners->Get()->pop_back();
	}
	return Result;
}
