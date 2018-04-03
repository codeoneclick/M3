// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SpawnModel.h"

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

void M3SpawnModel::PushSuperElementSpawner(const M3CellModel_SharedPtr& Spawner) {
	Entity->Get()->SuperElementSpawners->Get()->push_back(Spawner);
}

M3CellModel_SharedPtr M3SpawnModel::PopSuperElementSpawner() {
	M3CellModel_SharedPtr Result = nullptr;
	if (IsSuperElementSpawnersExist()) {
		Result = Entity->Get()->SuperElementSpawners->Get()->back();
		Entity->Get()->SuperElementSpawners->Get()->pop_back();
	}
	return Result;
}
