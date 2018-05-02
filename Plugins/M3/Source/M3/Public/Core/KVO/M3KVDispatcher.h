// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3KVBinding.h"
#include "M3Utilities.h"

class M3KVDispatcher : public M3KVBinding {
protected:

	std::vector<M3KVListener_INTERFACE_SharedPtr> Listeners;

public:

	virtual ~M3KVDispatcher() = default;

	void Subscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) {
		Listeners.push_back(Listener);
	}

	void Unsubscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) {
		const auto& It = std::find(Listeners.begin(), Listeners.end(), Listener);
		if (It != Listeners.end()) {
			Listeners.erase(It);
		} else {
			assert(false);
		}
	}

	void UnsubscribeAll() {
		Listeners.clear();
	}
};
