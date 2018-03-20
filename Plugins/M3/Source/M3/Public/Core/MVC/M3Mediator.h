// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3GlobalDispatcher.h"

FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)

class M3Mediator_INTERFACE : public std::enable_shared_from_this<M3Mediator_INTERFACE> {

public:

	M3Mediator_INTERFACE() = default;
	virtual ~M3Mediator_INTERFACE() = default;

	virtual void Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) = 0;
	virtual void Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) = 0;
};

class M3Mediator : public M3Mediator_INTERFACE {

public:

	M3Mediator() = default;
	~M3Mediator() = default;

	virtual void Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
		M3GlobalDispatcher::GetInstance()->Subscribe(Event);
	};

	virtual void Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(Event);
	};
};
