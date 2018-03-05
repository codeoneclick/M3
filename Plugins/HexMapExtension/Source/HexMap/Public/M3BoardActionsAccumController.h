// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

class HEXMAP_API M3BoardActionsAccumController : public M3MediatingController
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
