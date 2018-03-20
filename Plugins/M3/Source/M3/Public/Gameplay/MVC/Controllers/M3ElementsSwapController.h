// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3ElementsSwapController : public M3MediatingController
{
protected:

	class AActor* InteractionView = nullptr;

public:

	CTTI_CLASS_GUID(M3ElementsSwapController, M3MediatingController_INTERFACE::GuidsContainer)

	M3ElementsSwapController();
	~M3ElementsSwapController();

	void SetInteractionView(class AActor* InteractionView);

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
