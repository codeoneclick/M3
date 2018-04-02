// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3ElementModel.h"

FORWARD_DECL_STRONG(M3ElementModel)

class M3_API M3PotentialSwapEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3PotentialSwapEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3PotentialSwapEntity, SwapElementA, M3ElementModel_SharedPtr, nullptr)
	PROP_STRONG(public, M3PotentialSwapEntity, SwapElementB, M3ElementModel_SharedPtr, nullptr)
	PROP_STRONG(public, M3PotentialSwapEntity, Weightning, int, 0)
};

class M3_API M3PotentialSwapModel : public M3Model<M3PotentialSwapEntity> {
public:

	M3PotentialSwapModel();
	~M3PotentialSwapModel();

	CTTI_CLASS_GUID(M3PotentialSwapModel, M3Model_INTERFACE::GuidsContainer)

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init() override;

	PROP_DECL_R(SwapElementA, M3ElementModel_SharedPtr)
	PROP_DECL_R(SwapElementB, M3ElementModel_SharedPtr)

	PROP_DECL_W(SwapElementA, M3ElementModel_SharedPtr)
	PROP_DECL_W(SwapElementB, M3ElementModel_SharedPtr)
};

FORWARD_DECL_STRONG(M3CellModel)
FORWARD_DECL_STRONG(M3PotentialSwapModel)

class M3_API M3SwapEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3SwapEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3SwapEntity, SwapElementA, M3ElementModel_SharedPtr, nullptr)
	PROP_STRONG(public, M3SwapEntity, SwapElementB, M3ElementModel_SharedPtr, nullptr)
	PROP_STRONG(public, M3SwapEntity, PotentialSwaps, std::shared_ptr<std::vector<M3PotentialSwapModel_SharedPtr>>, std::make_shared<std::vector<M3PotentialSwapModel_SharedPtr>>());
};

class M3_API M3SwapModel : public M3Model<M3SwapEntity> {
private:

	static bool CanSwapElementWithElementColor(const std::shared_ptr<std::vector<M3CellModel_SharedPtr>>& Cells, int Cols, int Rows, int Col, int Row, EM3ElementColor Color);
	static bool CanSwapElements(const M3ElementModel_SharedPtr& ElementA, const M3ElementModel_SharedPtr& ElementB);

	bool HasChain(const std::shared_ptr<std::vector<M3CellModel_SharedPtr>>& Cells, int Cols, int Rows, int Col, int Row);

public:

	CTTI_CLASS_GUID(M3SwapModel, M3Model_INTERFACE::GuidsContainer)

	M3SwapModel();
	~M3SwapModel();

	PROP_DECL_R(SwapElementA, M3ElementModel_SharedPtr)
	PROP_DECL_R(SwapElementB, M3ElementModel_SharedPtr)
	PROP_DECL_R(PotentialSwaps, std::shared_ptr<std::vector<M3PotentialSwapModel_SharedPtr>>)

	static void SwapElements(const M3CellModel_SharedPtr& CellA, const M3CellModel_SharedPtr& CellB);
	static void SwapElements(const M3ElementModel_SharedPtr& ElementA, const M3ElementModel_SharedPtr& ElementB);

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init() override;

	void GeneratePotentialSwaps();
	void AddSwapElement(const M3ElementModel_SharedPtr& Element);
	void ResetSwapElements();
	bool IsPossibleToSwap() const;
};
