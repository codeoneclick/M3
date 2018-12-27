// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3RegularElementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3RegularElementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SharedModel.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3BoardSettingsModel.h"
#include "M3RegularElement.h"

namespace REGULAR_ELEMENT_VIEW_CONNECTION {
	const std::string k_ON_ELEMENT_ID_CHANGED = "ON_ELEMENT_ID_CHANGED";
	const std::string k_ON_ASSIGNED_STATE_CHANGED = "ON_ASSIGNED_STATE_CHANGED";
}

M3RegularElementView::M3RegularElementView(AActor* _Superview) : M3View(_Superview) {
}

M3RegularElementView::~M3RegularElementView() {
}

void M3RegularElementView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3RegularElementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto RegularElementModel = std::static_pointer_cast<M3RegularElementModel>(_ViewModel);
	RegularElementModel->Entity->Get()->IsAssignedToView->Set(true);
	std::shared_ptr<M3KVSlot<EM3ElementId>> OnElementIdChangedSlot = std::make_shared<M3KVSlot<EM3ElementId>>(RegularElementModel->Entity->Get()->Id);
	Slots[REGULAR_ELEMENT_VIEW_CONNECTION::k_ON_ELEMENT_ID_CHANGED] = OnElementIdChangedSlot;
	OnElementIdChangedSlot->Attach([=](EM3ElementId Id) {
		if (Id != EM3ElementId::UNKNOWN) {
			SetElementVisual(Id);
		}
	});
	SetElementVisual(RegularElementModel->Entity->Get()->Id->Get());

	std::shared_ptr<M3KVSlot<bool>> OnAssignedStateChangedSlot = std::make_shared<M3KVSlot<bool>>(RegularElementModel->Entity->Get()->IsAssignedToView);
	Slots[REGULAR_ELEMENT_VIEW_CONNECTION::k_ON_ASSIGNED_STATE_CHANGED] = OnAssignedStateChangedSlot;
	OnAssignedStateChangedSlot->Attach([=](bool IsAssignedToView) {
		if (!IsAssignedToView) {
			Dispose<AM3RegularElement>();
		}
	});
}

void M3RegularElementView::SetElementVisual(EM3ElementId Id) {
	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (Id)
		{
		case EM3ElementId::ELEMENT_RED:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
			break;
		case EM3ElementId::ELEMENT_GREEN:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
			break;
		case EM3ElementId::ELEMENT_BLUE:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
			break;
		case EM3ElementId::ELEMENT_YELLOW:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
			break;
		case EM3ElementId::ELEMENT_ORANGE:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
			break;
		case EM3ElementId::ELEMENT_PURPLE:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
			break;
		default:
			break;
		}
	}
}
