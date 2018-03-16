// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

class HEXMAP_API M3CellEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3CellEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3CellEntity, Col, int, 0)
	PROP_STRONG(public, M3CellEntity, Row, int, 0)
};

class HEXMAP_API M3CellModel : public M3Model<M3CellEntity>
{
public:
	M3CellModel();
	~M3CellModel();

	CTTI_CLASS_GUID(M3CellModel, M3Model<M3CellEntity>::GuidsContainer)

	PROP_DECL_R(Col, int);
	PROP_DECL_R(Row, int);

	void Init();

	void Serialize();
	void Deserialize(AM3Scheme_INTERFACE* Scheme);

	bool IsContainElement();
	bool CanContainElement();
};
