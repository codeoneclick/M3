// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SharedModel.h"

M3SharedModel_SharedPtr M3SharedModel::Instance = nullptr;

M3SharedModel::M3SharedModel() {
}

M3SharedModel::~M3SharedModel() {
}

M3SharedModel_SharedPtr M3SharedModel::GetInstance() {
	if (Instance == nullptr) {
		Instance = std::make_shared<M3SharedModel>();
		M3SharedModel::Register();
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
