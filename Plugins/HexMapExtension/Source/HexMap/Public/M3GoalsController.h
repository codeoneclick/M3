// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)

class HEXMAP_API M3GoalsController : public M3MediatingController
{
private:

protected:

	void OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);

public:

	CTTI_CLASS_GUID(M3GoalsController, M3MediatingController_INTERFACE::GuidsContainer)

	M3GoalsController();
	~M3GoalsController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
