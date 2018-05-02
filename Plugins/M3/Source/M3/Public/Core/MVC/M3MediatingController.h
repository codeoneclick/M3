// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Mediator.h"

class M3_API M3MediatingController_INTERFACE : public M3Mediator {
public:

	M3MediatingController_INTERFACE();
	virtual ~M3MediatingController_INTERFACE();

	virtual bool CanBeExecuted() const = 0;
	virtual void Execute(float Deltatime) = 0;

	CTTI_CLASS_GUID(M3MediatingController_INTERFACE)
};

FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)

class M3_API M3MediatingController : public M3MediatingController_INTERFACE
{
protected:

	std::vector<M3AppEvent_INTERFACE_SharedPtr> Events;

public:

	M3MediatingController();
	virtual ~M3MediatingController();

	void Subscribe(M3AppEvent_INTERFACE_SharedPtr Event);
	void Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event);
	void UnsubscribeAll();
};
