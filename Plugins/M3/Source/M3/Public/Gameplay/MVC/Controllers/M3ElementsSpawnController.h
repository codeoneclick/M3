// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3ElementsSpawnController : public M3MediatingController {
public:

	CTTI_CLASS_GUID(M3ElementsSpawnController)

	M3ElementsSpawnController();
	~M3ElementsSpawnController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
