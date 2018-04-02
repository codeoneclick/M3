// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"
#include "M3Scheme.h"

class M3BoardSettingsEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardSettingsEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3BoardSettingsEntity, Duration, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, IsTurnBased, bool, false)
	PROP_STRONG(public, M3BoardSettingsEntity, OneMatchScores, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, IsUseComboMatchScores, bool, false)
	PROP_STRONG(public, M3BoardSettingsEntity, ComboMatchScoresMultiplier, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, Star1Scores, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, Star2Scores, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, Star3Scores, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, ElementSize, FVector2D, FVector2D(-1, -1))
	PROP_STRONG(public, M3BoardSettingsEntity, ElementIds, std::shared_ptr<std::vector<EM3ElementId>>, std::make_shared<std::vector<EM3ElementId>>())
};

class M3_API M3BoardSettingsModel : public M3Model<M3BoardSettingsEntity>
{
public:

	CTTI_CLASS_GUID(M3BoardSettingsModel, M3Model_INTERFACE::GuidsContainer)

	M3BoardSettingsModel();
	~M3BoardSettingsModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	PROP_DECL_R(Duration, int)
	PROP_DECL_R(IsTurnBased, bool)
	PROP_DECL_R(OneMatchScores, int)
	PROP_DECL_R(IsUseComboMatchScores, bool)
	PROP_DECL_R(ComboMatchScoresMultiplier, int)
	PROP_DECL_R(Star1Scores, int)
	PROP_DECL_R(Star2Scores, int)
	PROP_DECL_R(Star3Scores, int)
	PROP_DECL_R(ElementSize, FVector2D)
	PROP_DECL_R(ElementIds, std::shared_ptr<std::vector<EM3ElementId>>)
};
