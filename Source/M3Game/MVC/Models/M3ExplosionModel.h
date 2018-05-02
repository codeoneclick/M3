// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"

class M3_API M3ExplosionEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3ExplosionEntity)

	PROP_STRONG(public, M3ExplosionEntity, Col, int, -1)
	PROP_STRONG(public, M3ExplosionEntity, Row, int, -1)
};

class M3_API M3ExplosionModel : public M3Model<M3ExplosionEntity> {
public:

	M3ExplosionModel() = default;
	~M3ExplosionModel() = default;

	void Init() override {

	}

	void Serialize() override {

	}

	void Deserialize(AM3Scheme_INTERFACE* Scheme) override {

	}

	PROP_DECL_R(Col, int)
	PROP_DECL_R(Row, int)
};
