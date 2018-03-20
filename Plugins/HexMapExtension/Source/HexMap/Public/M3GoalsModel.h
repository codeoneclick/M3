// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"

struct M3Goal {
public:

	EM3ElementId Id;
	int Quantity;
	int MaxQuantity;
};


class M3GoalsEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3GoalsEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3GoalsEntity, Goals, std::shared_ptr<std::vector<M3Goal>>, std::make_shared<std::vector<M3Goal>>())

};

class HEXMAP_API M3GoalsModel : public M3Model<M3GoalsEntity>
{
public:

	M3GoalsModel();
	~M3GoalsModel();

	CTTI_CLASS_GUID(M3GoalsModel, M3Model_INTERFACE::GuidsContainer)

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init();

	PROP_DECL_R(Goals, std::shared_ptr<std::vector<M3Goal>>)
};
