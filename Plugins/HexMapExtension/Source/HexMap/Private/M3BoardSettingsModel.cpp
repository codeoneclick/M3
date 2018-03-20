// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardSettingsModel.h"
#include "M3Scheme.h"

M3BoardSettingsModel::M3BoardSettingsModel() {
}

M3BoardSettingsModel::~M3BoardSettingsModel() {
}

void M3BoardSettingsModel::Init() {

}

void M3BoardSettingsModel::Serialize() {

}

void M3BoardSettingsModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {
	const auto BoardScheme = static_cast<AM3BoardScheme*>(Scheme);
	Entity->Get()->ElementSize->Set(BoardScheme->ElementSize);
	for (const auto ElementId : BoardScheme->ElementIds) {
		Entity->Get()->ElementIds->Get()->push_back(ElementId);
	}
	Entity->Get()->Turns->Set(BoardScheme->Turns);
}
