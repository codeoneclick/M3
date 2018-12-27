// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h" 

FORWARD_DECL_STRONG(M3ElementModel)

class M3_API M3AutobotEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3AutobotEntity)

	PROP_STRONG(public, M3CellEntity, Enabled, bool, false)
	PROP_STRONG(public, M3CellEntity, AnimationSpeed, float, .0001f)
	PROP_STRONG(public, M3CellEntity, MatchWeightning, float, 1.f)
	PROP_STRONG(public, M3CellEntity, GoalWeightning, float, 3.f)
};

class M3_API M3AutobotModel : public M3Model<M3AutobotEntity> {
public:

	M3AutobotModel();
	~M3AutobotModel();

	PROP_DECL_R(Enabled, bool);
	PROP_DECL_R(AnimationSpeed, float);

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	float GetMatchWeightning(int Count, int HorizonatlStart, int HorizontalEnd, int VerticalStart, int VerticalEnd, int Col, int Row);
	float GetSuperElementWeightning(EM3ElementId SuperElementId, int Col, int Row, int* OutColToSwap, int* OutRowToSwap, std::shared_ptr<std::set<M3ElementModel_SharedPtr>> IgnoredRegularElements = nullptr, std::shared_ptr<std::set<M3ElementModel_SharedPtr>> IgnoredSuperElements = nullptr);
};
