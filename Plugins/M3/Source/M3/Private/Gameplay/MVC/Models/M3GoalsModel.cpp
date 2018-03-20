// Fill out your copyright notice in the Description page of Project Settings.

#include "M3GoalsModel.h"

M3GoalsModel::M3GoalsModel() {

}

M3GoalsModel::~M3GoalsModel() {

}

void M3GoalsModel::Serialize() {

}

void M3GoalsModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	const auto BoardScheme = static_cast<AM3BoardScheme*>(Scheme);
	for (const auto Goal : BoardScheme->Goals) {
		if (Goal->Enabled && Goal->Quantity != 0) {
			Entity->Get()->Goals->Get()->push_back({ Goal->Id, 0, Goal->Quantity });
		}
	}
}

void M3GoalsModel::Init() {

}
