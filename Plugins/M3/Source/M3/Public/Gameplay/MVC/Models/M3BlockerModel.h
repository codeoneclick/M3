// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h" 

class M3BlockerEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BlockerEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3BlockerEntity, Id, EM3ElementId, EM3ElementId::UNKNOWN)
};

class M3_API M3BlockerModel : public M3Model<M3BlockerEntity> {
public:

	CTTI_CLASS_GUID(M3BlockerModel, M3Model_INTERFACE::GuidsContainer)

	M3BlockerModel();
	~M3BlockerModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Reset() override;

	PROP_DECL_R(Id, EM3ElementId)
};
