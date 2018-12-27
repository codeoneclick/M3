// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"
#include "M3ElementModel.h"

class M3BlockerEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BlockerEntity)

	PROP_STRONG(public, M3BlockerEntity, Id, EM3ElementId, EM3ElementId::UNKNOWN)
	PROP_STRONG(public, M3BlockerEntity, Density, int, 0)
	PROP_STRONG(public, M3BlockerEntity, AffectedOnTurn, int, 0)
};

class M3_API M3BlockerModel : public M3Model<M3BlockerEntity>, public M3ElementModelBlockerComponent_INTERFACE {
public:

	M3BlockerModel();
	~M3BlockerModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Reset() override;

	void DecDensity();

	bool CanDrop() override;
	bool CanSwap() override;
	bool CanMatch() override;
	bool CanRemove() override;

	PROP_DECL_R(Id, EM3ElementId)
	PROP_DECL_R(Density, int)
};
