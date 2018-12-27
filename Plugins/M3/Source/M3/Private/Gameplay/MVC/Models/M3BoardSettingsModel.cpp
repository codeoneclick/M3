// Copyright serhii serhiiv 2018 All rights reserved.

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
	Entity->Get()->Duration->Set(BoardScheme->Duration);
	Entity->Get()->IsTurnBased->Set(BoardScheme->IsTurnBased);
	Entity->Get()->OneMatchScores->Set(BoardScheme->OneMatchScores);
	Entity->Get()->IsUseComboMatchScores->Set(BoardScheme->IsUseComboMatchScores);
	Entity->Get()->ComboMatchScoresMultiplier->Set(BoardScheme->ComboMatchScoresMultiplier);
	Entity->Get()->Star1Scores->Set(BoardScheme->Star1Scores);
	Entity->Get()->Star2Scores->Set(BoardScheme->Star2Scores);
	Entity->Get()->Star3Scores->Set(BoardScheme->Star3Scores);
}
