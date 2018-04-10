// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperElementModel.h"

M3SuperElementModel::M3SuperElementModel() {
}

M3SuperElementModel::~M3SuperElementModel() {
}

void M3SuperElementModel::Init() {

}

void M3SuperElementModel::Serialize() {

}

void M3SuperElementModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3SuperElementModel::Reset() {
	M3Model::Reset();

	Entity->Get()->Id->Set(EM3ElementId::UNKNOWN);
}

int M3SuperElementModel::GetExplosionRadius(EM3ElementId Id) {
	switch (Id) {
	case EM3ElementId::SUPERELEMENT_MATCH4:
		return 1;
	case EM3ElementId::SUPERELEMENT_MATCH5:
		return 2;
	case EM3ElementId::SUPERELEMENT_MATCH6:
		return 3;
	case EM3ElementId::SUPERELEMENT_MATCH7:
		return 4;
	}
	return 0;
}