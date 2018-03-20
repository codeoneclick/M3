// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"

class M3SharedEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3SharedEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3SharedEntity, Version, int, 0)
};

FORWARD_DECL_STRONG(M3SharedModel)

class M3_API M3SharedModel : public M3Model<M3SharedEntity>
{
private:

	static M3SharedModel_SharedPtr Instance;

public:

	M3SharedModel();
	~M3SharedModel();

	static M3SharedModel_SharedPtr GetInstance();

	CTTI_CLASS_GUID(M3SharedModel, M3Model_INTERFACE::GuidsContainer)

	void Init();

	void Serialize();
	void Deserialize(AM3Scheme_INTERFACE* Scheme);
};
