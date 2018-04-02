// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3CoordinatingController.h"
#include "M3MediatingController.h"
#include "M3View.h"

M3CoordinatingController::M3CoordinatingController() {
}

M3CoordinatingController::~M3CoordinatingController() {
}

void M3CoordinatingController::OnUpdate(float Deltatime) {
	for (const auto& Controller : Controllers) {
		if (Controller->CanBeExecuted()) {
			Controller->Execute(Deltatime);
		}
	}
}

void M3CoordinatingController::AddView(const M3View_INTERFACE_SharedPtr& View) {
	const auto Id = View->InstanceGuid();
	const auto& It = std::find_if(Views.begin(), Views.end(), [Id](const M3View_INTERFACE_SharedPtr& View) {
		return View->InstanceGuid() == Id;
	});
	if (It == Views.end()) {
		Views.push_back(View);
	} else {
		assert(false);
	}
}

void M3CoordinatingController::RemoveView(const M3View_INTERFACE_SharedPtr& View) {
	const auto& It = std::find(Views.begin(), Views.end(), View);
	if (It != Views.end()) {
		Views.erase(It);
	}
}

void M3CoordinatingController::RemoveView(int Id) {
	const auto& It = std::find_if(Views.begin(), Views.end(), [Id](const M3View_INTERFACE_SharedPtr& View) {
		return View->InstanceGuid() == Id;
	});
	if (It != Views.end()) {
		Views.erase(It);
	}
}

M3View_INTERFACE_SharedPtr M3CoordinatingController::GetView(int Id) {
	const auto& It = std::find_if(Views.begin(), Views.end(), [Id](const M3View_INTERFACE_SharedPtr& View) {
		return View->InstanceGuid() == Id;
	});
	return (*It);
}

void M3CoordinatingController::AddController(const M3MediatingController_INTERFACE_SharedPtr& Controller) {
	const auto Id = Controller->InstanceGuid();
	const auto& It = std::find_if(Controllers.begin(), Controllers.end(), [Id](const M3MediatingController_INTERFACE_SharedPtr& Controller) {
		return Controller->InstanceGuid() == Id;
	});
	if (It == Controllers.end()) {
		Controllers.push_back(Controller);
	}
	else {
		assert(false);
	}
}

void M3CoordinatingController::RemoveController(const M3MediatingController_INTERFACE_SharedPtr& Controller) {
	const auto& It = std::find(Controllers.begin(), Controllers.end(), Controller);
	if (It != Controllers.end()) {
		Controllers.erase(It);
	}
}

void M3CoordinatingController::RemoveController(int Id) {
	const auto& It = std::find_if(Controllers.begin(), Controllers.end(), [Id](const M3MediatingController_INTERFACE_SharedPtr& Controller) {
		return Controller->InstanceGuid() == Id;
	});
	if (It != Controllers.end()) {
		Controllers.erase(It);
	}
}

M3MediatingController_INTERFACE_SharedPtr M3CoordinatingController::GetController(int Id){
	const auto& It = std::find_if(Controllers.begin(), Controllers.end(), [Id](const M3MediatingController_INTERFACE_SharedPtr& Controller) {
		return Controller->InstanceGuid() == Id;
	});
	return (*It);
}
