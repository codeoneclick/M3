// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SharedModel.h"

M3SharedModel_SharedPtr M3SharedModel::Instance = nullptr;

M3SharedModel::M3SharedModel() {
	Containers.fill(nullptr);
	TempContainers.fill(nullptr);
}

M3SharedModel::~M3SharedModel() {
}

M3SharedModel_SharedPtr M3SharedModel::GetInstance() {
	if (Instance == nullptr) {
		Instance = std::make_shared<M3SharedModel>();
	}
	return Instance;
};

void M3SharedModel::Init() {
	SUBSCRIBE_PROP(M3SharedEntity, Version)
}

void M3SharedModel::Serialize() {

}

void M3SharedModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	for (const auto& Submodel : Submodels) {
		if (Submodel) {
			Submodel->Deserialize(Scheme);
		}
	}
}

void M3SharedModel::UnregisterAllContainers() {
	for (const auto It : Containers) {
		if (It) {
			It->Get()->clear();
		}
	}
	Containers.fill(nullptr);

	for (const auto It : TempContainers) {
		if (It) {
			It->clear();
		}
	}
	TempContainers.fill(nullptr);

	for (const auto It : Pools) {
		if (It) {
			It->clear();
		}
	}
	Pools.fill(nullptr);
}

std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> M3SharedModel::Container(uintptr_t Guid) {
	return Containers[Guid];
}

std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> M3SharedModel::TempContainer(uintptr_t Guid) {
	return TempContainers[Guid];
}

std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> M3SharedModel::Pool(uintptr_t Guid) {
	return Pools[Guid];
}
