// Fill out your copyright notice in the Description page of Project Settings.

#include "M3View.h"
#include "M3Model.h"
#include "M3Entity.h"
#include "M3KVSlot.h"
#include "M3ViewDelegate.h"
#include "M3ViewDelegates_API.h"
#include "Engine/World.h"

std::set<uintptr_t> M3View_INTERFACE::GuidsContainer;

M3View::M3View(AActor* Superview) {
	this->Superview = Superview;
}

M3View::~M3View() {
	Superview = nullptr;
	Parent.reset();

	for (const auto& Slot : Slots) {
		Slot.second->DetachAll();
	}
	Slots.clear();

	Subviews.clear();
}

AActor* M3View::GetSuperview() const {
	return Superview;
}

void M3View::Load(AM3AssetsBundle* _Bundle) {
	this->Bundle = _Bundle;
}

void M3View::BindViewDelegates(AM3ViewDelegates_API* API) {
	Delegates = API;
	const auto DelegateBP = API->GetDelegate(InstanceGuid());
	if (DelegateBP) {
		Delegate = NewObject<UM3ViewDelegate_INTERFACE>(Superview, DelegateBP);
	}
}

void M3View::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	this->ViewModel = _ViewModel;

	const auto Entity = GetViewModel<M3Model<M3Entity>>()->Entity->Get();
	assert(Entity->IsAssignedToView->Get() == false);
	Entity->IsAssignedToView->Set(true);
}

void M3View::AddSubview(const M3View_INTERFACE_SharedPtr& Subview) {
	Subviews.push_back(Subview);
	std::static_pointer_cast<M3View>(Subview)->Parent = shared_from_this();
}

void M3View::RemoveSubview(const M3View_INTERFACE_SharedPtr& Subview) {
	const auto It = std::find(Subviews.begin(), Subviews.end(), Subview);
	if (It != Subviews.end()) {
		Subviews.erase(It);
		std::static_pointer_cast<M3View>(Subview)->Parent.reset();
	}
}

void M3View::RemoveFromParent() {
	if (!Parent.expired()) {
		std::static_pointer_cast<M3View>(Parent.lock())->RemoveSubview(shared_from_this());
	}
}
