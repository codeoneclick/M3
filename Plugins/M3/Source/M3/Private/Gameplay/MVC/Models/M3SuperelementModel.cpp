// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperelementModel.h"

M3SuperelementModel::M3SuperelementModel() {
}

M3SuperelementModel::~M3SuperelementModel() {
}

void M3SuperelementModel::Init() {

}

void M3SuperelementModel::Serialize() {

}

void M3SuperelementModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3SuperelementModel::Reset() {
	M3Model::Reset();

	Entity->Get()->Id->Set(EM3SuperelementId::UNKNOWN);
}