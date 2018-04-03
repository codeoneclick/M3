// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"

class M3SuperelementEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3SuperelementEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3SuperelementEntity, Id, EM3SuperelementId, EM3SuperelementId::UNKNOWN)
};

class M3_API M3SuperelementModel : public M3Model<M3SuperelementEntity> {
public:

	CTTI_CLASS_GUID(M3SuperelementModel, M3Model_INTERFACE::GuidsContainer)

	M3SuperelementModel();
	~M3SuperelementModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Reset() override;

	PROP_DECL_R(Id, EM3SuperelementId)
};
