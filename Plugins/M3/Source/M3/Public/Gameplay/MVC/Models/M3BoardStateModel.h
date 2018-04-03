// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"

FORWARD_DECL_STRONG(M3ElementModel)

class M3BoardStateEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardStateEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3ElementEntity, Timestamp, int64, 0)
	PROP_STRONG(public, M3BoardStateEntity, IsHolesExist, bool, true)
	PROP_STRONG(public, M3BoardStateEntity, Turn, int, 0)
	PROP_STRONG(public, M3BoardStateEntity, Duration, int, 0)
	PROP_STRONG(public, M3BoardStateEntity, Scores, int, 0)
};

class M3_API M3BoardStateModel : public M3Model<M3BoardStateEntity>
{

public:

	CTTI_CLASS_GUID(M3BoardStateModel, M3Model_INTERFACE::GuidsContainer)

	M3BoardStateModel();
	~M3BoardStateModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	PROP_DECL_R(Timestamp, int64)
	PROP_DECL_R(IsHolesExist, bool)
	PROP_DECL_W(IsHolesExist, bool)
	PROP_DECL_R(Turn, int)
	PROP_DECL_R(Duration, int)
	PROP_DECL_R(Scores, int)

	void IncGameTurn();
	void IncTimestamp();
};
