// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

FORWARD_DECL_STRONG(M3MediatingController_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)

class M3_API M3CoordinatingController
{
protected:

	std::vector<M3MediatingController_INTERFACE_SharedPtr> Controllers;
	std::vector<M3View_INTERFACE_SharedPtr> Views;

public:

	M3CoordinatingController();
	~M3CoordinatingController();

	void OnUpdate(float Deltatime);

	void AddView(const M3View_INTERFACE_SharedPtr& View);
	void RemoveView(const M3View_INTERFACE_SharedPtr& View);
	void RemoveView(int Id);
	M3View_INTERFACE_SharedPtr GetView(int Id);

	void AddController(const M3MediatingController_INTERFACE_SharedPtr& Controller);
	void RemoveController(const M3MediatingController_INTERFACE_SharedPtr& Controller);
	void RemoveController(int Id);
	M3MediatingController_INTERFACE_SharedPtr GetController(int Id);

	template<typename T> 
	std::shared_ptr<T> GetControllerAs() {
		return std::static_pointer_cast<T>(GetController(T::ClassGuid()));
	};
};
