// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Scheme.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)
FORWARD_DECL_STRONG(M3SuperElementModel)
FORWARD_DECL_STRONG(M3ElementModel)

class M3_API M3SuperElementsController : public M3MediatingController {
protected:

	std::queue<M3SuperElementModel_SharedPtr> SuperElementsToExecute;

	void OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);
	void OnSwapEnded(const M3ElementModel_SharedPtr& ElementModel, bool IsPossibleToSwap);

	void OnExplosion(int Col, int Row, int Radius);

public:

	CTTI_CLASS_GUID(M3SuperElementsController, M3MediatingController_INTERFACE::GuidsContainer)

	M3SuperElementsController();
	~M3SuperElementsController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
