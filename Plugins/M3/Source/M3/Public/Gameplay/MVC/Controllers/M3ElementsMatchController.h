// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3ElementsMatchController : public M3MediatingController
{
protected:

	int LastExecutedTurn = 0;

public:

	CTTI_CLASS_GUID(M3ElementsMatchController)

	M3ElementsMatchController();
	~M3ElementsMatchController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
