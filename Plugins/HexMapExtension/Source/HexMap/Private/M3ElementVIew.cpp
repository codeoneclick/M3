// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementView.h"
#include "M3KVSlot.h"
#include "M3ElementModel.h"

const std::string k_ON_ELEMENT_STATE_CHANGED = "ON_ELEMENT_STATE_CHANGED";
const std::string k_ON_ELEMENT_ID_CHANGED = "ON_ELEMENT_ID_CHANGED";

M3ElementView::M3ElementView(AActor* Superview) : M3View(Superview)
{

}

void M3ElementView::Load(AM3AssetsBundle* Bundle) {
	M3View::Load(Bundle);
}

void M3ElementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) {
	M3View::BindViewModel(ViewModel);
	const auto& ElementModel = GetViewModel<M3ElementModel>();
	const auto& ElementEntity = ElementModel->Entity->Get();

	std::shared_ptr<M3KVSlot<EM3ElementState>> OnElementStateChangedSlot = std::make_shared<M3KVSlot<EM3ElementState>>(ElementEntity->State);
	Slots[k_ON_ELEMENT_STATE_CHANGED] = OnElementStateChangedSlot;
	OnElementStateChangedSlot->Attach([] (EM3ElementState State) {
		// TODO:
	});

	std::shared_ptr<M3KVSlot<int>> OnElementIdChangedSlot = std::make_shared<M3KVSlot<int>>(ElementEntity->ElementId);
	Slots[k_ON_ELEMENT_ID_CHANGED] = OnElementIdChangedSlot;
	OnElementIdChangedSlot->Attach([](int Id) {
		// TODO:
	});
}
