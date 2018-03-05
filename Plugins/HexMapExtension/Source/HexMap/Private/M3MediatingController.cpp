// Fill out your copyright notice in the Description page of Project Settings.

#include "M3MediatingController.h"

std::set<uintptr_t> M3MediatingController_INTERFACE::GuidsContainer;

M3MediatingController_INTERFACE::M3MediatingController_INTERFACE() {

}

M3MediatingController_INTERFACE::~M3MediatingController_INTERFACE() {

}

M3MediatingController::M3MediatingController() {

}

M3MediatingController::~M3MediatingController() {

}

void M3MediatingController::Subscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	M3Mediator::Subscribe(Event);
	Events.push_back(Event);
};

void M3MediatingController::Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event) {
	M3Mediator::Unsubscribe(Event);
	const auto It = std::find(Events.begin(), Events.end(), Event);
	if (It != Events.end()) {
		Events.erase(It);
	}
};