// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

FORWARD_DECL_STRONG(M3KVListener_INTERFACE)

class M3_API M3KVBinding
{
public:
	virtual void Subscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) = 0;
	virtual void Unsubscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) = 0;
	virtual void UnsubscribeAll() = 0;
};