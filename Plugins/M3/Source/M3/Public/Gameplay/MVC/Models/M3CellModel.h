// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

class M3_API M3CellEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3CellEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3CellEntity, Col, int, 0)
	PROP_STRONG(public, M3CellEntity, Row, int, 0)
	PROP_STRONG(public, M3CellEntity, IsClosed, bool, false)

};

class M3_API M3CellModel : public M3Model<M3CellEntity>
{
public:
	M3CellModel();
	~M3CellModel();

	CTTI_CLASS_GUID(M3CellModel, M3Model<M3CellEntity>::GuidsContainer)

	PROP_DECL_R(Col, int);
	PROP_DECL_R(Row, int);
	PROP_DECL_R(IsClosed, bool);

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	bool IsContainElement();
	bool CanContainElement();
};
