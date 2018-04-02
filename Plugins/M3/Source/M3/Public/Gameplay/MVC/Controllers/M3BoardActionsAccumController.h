// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3BoardActionsAccumController : public M3MediatingController
{
private:

protected:

public:

	CTTI_CLASS_GUID(M3BoardActionsAccumController, M3MediatingController_INTERFACE::GuidsContainer)

	M3BoardActionsAccumController();
	~M3BoardActionsAccumController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
