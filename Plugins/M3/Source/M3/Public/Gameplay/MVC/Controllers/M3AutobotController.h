// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3AutobotController : public M3MediatingController
{
public:

	CTTI_CLASS_GUID(M3AutobotController)

	M3AutobotController();
	~M3AutobotController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
