// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3BlockersController : public M3MediatingController {
public:

	CTTI_CLASS_GUID(M3BlockersController, M3MediatingController_INTERFACE::GuidsContainer)

	M3BlockersController();
	~M3BlockersController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
