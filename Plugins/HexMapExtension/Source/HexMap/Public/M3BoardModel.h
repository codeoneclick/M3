// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"
#include "M3CellModel.h"

FORWARD_DECL_STRONG(M3CellModel)

class M3BoardEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3BoardEntity, Cols, int, 0)
	PROP_STRONG(public, M3BoardEntity, Rows, int, 0)
	PROP_STRONG(public, M3BoardEntity, Cells, std::shared_ptr<std::vector<M3CellModel_SharedPtr>>, std::make_shared<std::vector<M3CellModel_SharedPtr>>())
};

class HEXMAP_API M3BoardModel : public M3Model<M3BoardEntity>
{
public:

	M3BoardModel();
	~M3BoardModel();

	CTTI_CLASS_GUID(M3BoardModel, M3Model_INTERFACE::GuidsContainer)

	void Init();

	void Serialize();
	void Deserialize(AM3Scheme_INTERFACE* Scheme);

	PROP_DECL_R(Cols, int)
	PROP_DECL_R(Rows, int)
	PROP_DECL_R(Cells, std::shared_ptr<std::vector<M3CellModel_SharedPtr>>)
};