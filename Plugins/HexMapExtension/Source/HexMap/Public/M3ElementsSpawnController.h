// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class HEXMAP_API M3ElementsSpawnController : public M3MediatingController
{
public:

	CTTI_CLASS_GUID(M3ElementsSpawnController, M3MediatingController_INTERFACE::GuidsContainer)

	M3ElementsSpawnController();
	~M3ElementsSpawnController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
