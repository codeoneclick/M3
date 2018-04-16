// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperElementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3SuperElementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SharedModel.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3BoardSettingsModel.h"
#include "M3SuperElement.h"

namespace SUPER_ELEMENT_VIEW_CONNECTION {
	const std::string k_ON_SUPERELEMENT_ID_CHANGED = "ON_SUPERELEMENT_ID_CHANGED";
	const std::string k_ON_ASSIGNED_STATE_CHANGED = "ON_ASSIGNED_STATE_CHANGED";
}

M3SuperElementView::M3SuperElementView(AActor* _Superview) : M3View(_Superview) {
}

M3SuperElementView::~M3SuperElementView() {
}

void M3SuperElementView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3SuperElementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto SuperElementModel = std::static_pointer_cast<M3SuperElementModel>(_ViewModel);
	SuperElementModel->Entity->Get()->IsAssignedToView->Set(true);
	std::shared_ptr<M3KVSlot<EM3ElementId>> OnSuperElementIdChangedSlot = std::make_shared<M3KVSlot<EM3ElementId>>(SuperElementModel->Entity->Get()->Id);
	Slots[SUPER_ELEMENT_VIEW_CONNECTION::k_ON_SUPERELEMENT_ID_CHANGED] = OnSuperElementIdChangedSlot;
	OnSuperElementIdChangedSlot->Attach([=](EM3ElementId Id) {
		if (Id != EM3ElementId::UNKNOWN) {
			SetElementVisual(Id);
		}
	});
	SetElementVisual(SuperElementModel->Entity->Get()->Id->Get());

	std::shared_ptr<M3KVSlot<bool>> OnAssignedStateChangedSlot = std::make_shared<M3KVSlot<bool>>(SuperElementModel->Entity->Get()->IsAssignedToView);
	Slots[SUPER_ELEMENT_VIEW_CONNECTION::k_ON_ASSIGNED_STATE_CHANGED] = OnAssignedStateChangedSlot;
	OnAssignedStateChangedSlot->Attach([=](bool IsAssignedToView) {
		if (!IsAssignedToView) {
			Dispose<AM3SuperElement>();
		}
	});
}

void M3SuperElementView::SetElementVisual(EM3ElementId Id) {
	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (Id)
		{
		case EM3ElementId::SUPERELEMENT_MATCH4:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH4.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH4.Material);
			break;
		case EM3ElementId::SUPERELEMENT_MATCH5:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH5.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH5.Material);
			break;
		case EM3ElementId::SUPERELEMENT_MATCH6:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH6.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH6.Material);
			break;
		case EM3ElementId::SUPERELEMENT_MATCH7:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH7.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH7.Material);
			break;
		default:
			break;
		}
	}
}
