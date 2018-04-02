// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3GlobalDispatcher.h"
#include "M3AppEvent.h"

M3GlobalDispatcher_SharedPtr M3GlobalDispatcher::Instance = nullptr;

void M3GlobalDispatcher::Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	const auto It = Listeners.find(Event->GetId());
	if (It == Listeners.end()) {
		Listeners.insert(std::make_pair(Event->GetId(), std::vector<M3AppEvent_INTERFACE_SharedPtr>()));
	}
	Listeners[Event->GetId()].push_back(Event);
};

void M3GlobalDispatcher::Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	const auto It1 = Listeners.find(Event->GetId());
	if (It1 != Listeners.end()) {
		const auto It2 = std::find(Listeners[Event->GetId()].begin(), Listeners[Event->GetId()].end(), Event);
		if (It2 != Listeners[Event->GetId()].end()) {
			Listeners[Event->GetId()].erase(It2);
		}
	}
};
