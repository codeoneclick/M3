// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3CoordinatingController.h"
#include "M3MediatingController.h"
#include "M3View.h"

M3CoordinatingController::M3CoordinatingController() {
}

M3CoordinatingController::~M3CoordinatingController() {
}

void M3CoordinatingController::OnUpdate(float Deltatime) {
	for (const auto& Controller : OrderedControllers) {
		if (Controller->CanBeExecuted()) {
			Controller->Execute(Deltatime);
		}
	}
}

void M3CoordinatingController::AddView(const M3View_INTERFACE_SharedPtr& View) {
	const auto& It = Views.find(View->InstanceGuid());
	if (It == Views.end()) {
		Views[View->InstanceGuid()] = View;
	} else {
		ensure(false);
	}
}

void M3CoordinatingController::RemoveView(const M3View_INTERFACE_SharedPtr& View) {
	RemoveView(View->InstanceGuid());
}

void M3CoordinatingController::RemoveView(uintptr_t Id) {
	const auto& It = Views.find(Id);
	if (It != Views.end()) {
		Views.erase(It);
	}
}

M3View_INTERFACE_SharedPtr M3CoordinatingController::GetView(uintptr_t Id) {
	M3View_INTERFACE_SharedPtr View = nullptr;
	const auto& It = Views.find(Id);
	if(It != Views.end()) {
		View = It->second;
	}
	return View;
}

void M3CoordinatingController::AddController(const M3MediatingController_INTERFACE_SharedPtr& Controller) {
	const auto Id = Controller->InstanceGuid();
	const auto& It = Controllers.find(Controller->InstanceGuid());
	if (It == Controllers.end()) {
		Controllers[Controller->InstanceGuid()] = Controller;
		OrderedControllers.push_back(Controller);
	} else {
		ensure(false);
	}
}

void M3CoordinatingController::RemoveController(const M3MediatingController_INTERFACE_SharedPtr& Controller) {
	RemoveController(Controller->InstanceGuid());
}

void M3CoordinatingController::RemoveController(uintptr_t Id) {
	const auto& It = Controllers.find(Id);
	if (It != Controllers.end()) {
		Controllers.erase(It);
	}
	OrderedControllers.erase(std::remove_if(OrderedControllers.begin(), OrderedControllers.end(), [=](const M3MediatingController_INTERFACE_SharedPtr& Controller) {
		return Controller->InstanceGuid() == Id;
	}), OrderedControllers.end());
}

M3MediatingController_INTERFACE_SharedPtr M3CoordinatingController::GetController(uintptr_t Id) {
	M3MediatingController_INTERFACE_SharedPtr Controller = nullptr;
	const auto& It = Controllers.find(Id);
	if (It != Controllers.end()) {
		Controller = It->second;
	}
	return Controller;
}
