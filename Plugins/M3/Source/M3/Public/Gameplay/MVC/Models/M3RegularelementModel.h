// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"
#include "M3ElementModel.h"

class M3RegularelementEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3RegularelementEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3RegularelementEntity, Id, EM3ElementId, EM3ElementId::UNKNOWN)
};

class M3_API M3RegularelementModel : public M3Model<M3RegularelementEntity>, public M3ElementModelColorComponent_INTERFACE
{
public:

	M3RegularelementModel();
	~M3RegularelementModel();

	CTTI_CLASS_GUID(M3RegularelementModel, M3Model_INTERFACE::GuidsContainer)

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Reset() override;

	EM3ElementColor GetColor();

	PROP_DECL_R(Id, EM3ElementId)
};