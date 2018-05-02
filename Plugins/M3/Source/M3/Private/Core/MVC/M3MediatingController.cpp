// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3MediatingController.h"

M3MediatingController_INTERFACE::M3MediatingController_INTERFACE() {

}

M3MediatingController_INTERFACE::~M3MediatingController_INTERFACE() {

}

M3MediatingController::M3MediatingController() {

}

M3MediatingController::~M3MediatingController() {
	UnsubscribeAll();
}

void M3MediatingController::Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	M3Mediator::Subscribe(Event);
	Events.push_back(Event);
}

void M3MediatingController::Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	M3Mediator::Unsubscribe(Event);
	const auto It = std::find(Events.begin(), Events.end(), Event);
	if (It != Events.end()) {
		Events.erase(It);
	}
}

void M3MediatingController::UnsubscribeAll() {
	for (const auto Event : Events) {
		M3Mediator::Unsubscribe(Event);
	}
	Events.clear();
}