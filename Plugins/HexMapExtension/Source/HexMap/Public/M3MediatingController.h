// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Mediator.h"

class M3MediatingController_INTERFACE : public M3Mediator {
protected:

	static std::set<uintptr_t> GuidsContainer;

public:

	M3MediatingController_INTERFACE() = default;
	virtual ~M3MediatingController_INTERFACE() = default;

	CTTI_CLASS_GUID(M3MediatingController_INTERFACE, M3MediatingController_INTERFACE::GuidsContainer)
};

FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)

class M3MediatingController : public M3MediatingController_INTERFACE
{
protected:

	std::vector<M3AppEvent_INTERFACE_SharedPtr> Events;

public:

	M3MediatingController() = default;
	~M3MediatingController() = default;

	void Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
		M3Mediator::Subscribe(Event);
		Events.push_back(Event);
	};

	void Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
		M3Mediator::Unsubscribe(Event);
		const auto It = std::find(Events.begin(), Events.end(), Event);
		if (It != Events.end()) {
			Events.erase(It);
		}
	};
};
