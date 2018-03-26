// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3GameStateController : public M3MediatingController {
protected:

	float InGameDuration = 0.f;

public:

	CTTI_CLASS_GUID(M3GameStateController, M3MediatingController_INTERFACE::GuidsContainer)

	M3GameStateController();
	~M3GameStateController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
