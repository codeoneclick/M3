// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"
#include "M3CellModel.h"

FORWARD_DECL_STRONG(M3CellModel)
FORWARD_DECL_STRONG(M3ElementModel)

class M3BoardEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardEntity)

	PROP_STRONG(public, M3BoardEntity, Cols, int, 0)
	PROP_STRONG(public, M3BoardEntity, Rows, int, 0)
	PROP_STRONG(public, M3BoardEntity, Cells, std::shared_ptr<std::vector<M3CellModel_SharedPtr>>, std::make_shared<std::vector<M3CellModel_SharedPtr>>())
};

class M3_API M3BoardModel : public M3Model<M3BoardEntity> {
public:

	M3BoardModel();
	~M3BoardModel();

	PROP_DECL_R(Cols, int)
	PROP_DECL_R(Rows, int)
	PROP_DECL_R(Cells, std::shared_ptr<std::vector<M3CellModel_SharedPtr>>)

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	M3CellModel_SharedPtr GetCell(int Col, int Row) const;
	M3ElementModel_SharedPtr GetElement(int Col, int Row) const;

	void CreateHole(int Col, int Row);
	void RemoveMatched();

	static void MoveElement(const M3CellModel_SharedPtr& From, const M3CellModel_SharedPtr& To);
};