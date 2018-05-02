// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"

class M3_API M3GoalEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3GoalEntity)

	PROP_STRONG(public, M3GoalEntity, Id, EM3GoalId, EM3GoalId::UNKNOWN)
	PROP_STRONG(public, M3GoalEntity, Index, int, 0)
	PROP_STRONG(public, M3GoalEntity, Quantity, int, 0)
	PROP_STRONG(public, M3GoalEntity, MaxQuantity, int, 0)
};

class M3_API M3GoalModel : public M3Model<M3GoalEntity> {
public:

	M3GoalModel();
	~M3GoalModel();

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init() override;

	PROP_DECL_R(Id, EM3GoalId)
	PROP_DECL_R(Index, int)
	PROP_DECL_R(Quantity, int)
	PROP_DECL_R(MaxQuantity, int)

	void IncQuantity();
	bool IsDone() const;
};

FORWARD_DECL_STRONG(M3GoalModel)


class M3_API M3GoalsEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3GoalsEntity)

	PROP_STRONG(public, M3GoalsEntity, Goals, std::shared_ptr<std::vector<M3GoalModel_SharedPtr>>, std::make_shared<std::vector<M3GoalModel_SharedPtr>>())

};

class M3_API M3GoalsModel : public M3Model<M3GoalsEntity> {
public:

	M3GoalsModel();
	~M3GoalsModel();

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init() override;

	PROP_DECL_R(Goals, std::shared_ptr<std::vector<M3GoalModel_SharedPtr>>)
};
