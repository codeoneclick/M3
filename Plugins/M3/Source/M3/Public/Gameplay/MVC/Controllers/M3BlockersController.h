// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3ElementModel)
FORWARD_DECL_STRONG(M3BlockerModel)

class M3_API M3BlockersController : public M3MediatingController {
private:

	std::set<M3BlockerModel_SharedPtr> BlockersToRemove;

	void OnElementMatchEnded(const M3ElementModel_SharedPtr& ElementModel);
	void OnBlockerAffected(const M3ElementModel_SharedPtr& ElementModel, const M3BlockerModel_SharedPtr& BlockerModel);

public:

	CTTI_CLASS_GUID(M3BlockersController, M3MediatingController_INTERFACE::GuidsContainer)

	M3BlockersController();
	~M3BlockersController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
