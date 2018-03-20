// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class M3_API M3ElementsMatchController : public M3MediatingController
{
public:

	CTTI_CLASS_GUID(M3ElementsMatchController, M3MediatingController_INTERFACE::GuidsContainer)

	M3ElementsMatchController();
	~M3ElementsMatchController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
