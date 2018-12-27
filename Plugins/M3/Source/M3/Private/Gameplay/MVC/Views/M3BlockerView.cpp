// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BlockerView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3BlockerModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SharedModel.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3BoardSettingsModel.h"
#include "M3Blocker.h"

namespace BLOCKER_VIEW_CONNECTION {
	const std::string k_ON_BLOCKER_ID_CHANGED = "ON_BLOCKER_ID_CHANGED";
	const std::string k_ON_ASSIGNED_STATE_CHANGED = "ON_ASSIGNED_STATE_CHANGED";
}

M3BlockerView::M3BlockerView(AActor* _Superview) : M3View(_Superview) {
}

M3BlockerView::~M3BlockerView() {
}

void M3BlockerView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3BlockerView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto BlockerModel = std::static_pointer_cast<M3BlockerModel>(_ViewModel);
	BlockerModel->Entity->Get()->IsAssignedToView->Set(true);
	std::shared_ptr<M3KVSlot<EM3ElementId>> OnBlockerIdChangedSlot = std::make_shared<M3KVSlot<EM3ElementId>>(BlockerModel->Entity->Get()->Id);
	Slots[BLOCKER_VIEW_CONNECTION::k_ON_BLOCKER_ID_CHANGED] = OnBlockerIdChangedSlot;
	OnBlockerIdChangedSlot->Attach([=](EM3ElementId Id) {
		if (Id != EM3ElementId::UNKNOWN) {
			SetElementVisual(Id);
		}
	});
	SetElementVisual(BlockerModel->Entity->Get()->Id->Get());

	std::shared_ptr<M3KVSlot<bool>> OnAssignedStateChangedSlot = std::make_shared<M3KVSlot<bool>>(BlockerModel->Entity->Get()->IsAssignedToView);
	Slots[BLOCKER_VIEW_CONNECTION::k_ON_ASSIGNED_STATE_CHANGED] = OnAssignedStateChangedSlot;
	OnAssignedStateChangedSlot->Attach([=](bool IsAssignedToView) {
		if (!IsAssignedToView) {
			Dispose<AM3Blocker>();
		}
	});
}

void M3BlockerView::SetElementVisual(EM3ElementId Id) {
	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (Id) {
		case EM3ElementId::BLOCKER_BOX1X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box1X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box1X.Material);
			break;
		case EM3ElementId::BLOCKER_BOX2X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box2X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box2X.Material);
			break;
		case EM3ElementId::BLOCKER_BOX3X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box3X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box3X.Material);
			break;
		case EM3ElementId::BLOCKER_ICE1X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Ice1X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Ice1X.Material);
			break;
		case EM3ElementId::BLOCKER_ICE2X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Ice2X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Ice2X.Material);
			break;
		case EM3ElementId::BLOCKER_WIRE1X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Wire1X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Wire1X.Material);
			break;
		case EM3ElementId::BLOCKER_WIRE2X:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Wire2X.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Wire2X.Material);
			break;
		default:
			break;
		}
	}
}
