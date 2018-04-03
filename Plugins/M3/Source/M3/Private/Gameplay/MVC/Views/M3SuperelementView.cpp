// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperelementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3SuperelementModel.h"
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

M3SuperelementView::M3SuperelementView(AActor* _Superview) : M3View(_Superview) {
}

M3SuperelementView::~M3SuperelementView() {
}

void M3SuperelementView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3SuperelementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto SuperElementModel = std::static_pointer_cast<M3SuperelementModel>(_ViewModel);
	SuperElementModel->Entity->Get()->IsAssignedToView->Set(true);
	std::shared_ptr<M3KVSlot<EM3SuperelementId>> OnSuperelementIdChangedSlot = std::make_shared<M3KVSlot<EM3SuperelementId>>(SuperElementModel->Entity->Get()->Id);
	Slots[SUPER_ELEMENT_VIEW_CONNECTION::k_ON_SUPERELEMENT_ID_CHANGED] = OnSuperelementIdChangedSlot;
	OnSuperelementIdChangedSlot->Attach([=](EM3SuperelementId Id) {
		if (Id != EM3SuperelementId::UNKNOWN) {
			SetSuperelementVisual(Id);
		}
	});
	SetSuperelementVisual(SuperElementModel->Entity->Get()->Id->Get());

	std::shared_ptr<M3KVSlot<bool>> OnAssignedStateChangedSlot = std::make_shared<M3KVSlot<bool>>(SuperElementModel->Entity->Get()->IsAssignedToView);
	Slots[SUPER_ELEMENT_VIEW_CONNECTION::k_ON_ASSIGNED_STATE_CHANGED] = OnAssignedStateChangedSlot;
	OnAssignedStateChangedSlot->Attach([=](bool IsAssignedToView) {
		if (!IsAssignedToView) {
			Dispose<AM3Superelement>();
			UE_LOG(LogTemp, Warning, TEXT("SuperElement should be destroyed!"));
		}
	});
}

void M3SuperelementView::SetSuperelementVisual(EM3SuperelementId Id) {
	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (Id)
		{
		case EM3SuperelementId::MATCH4_BOMB:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
			break;
		case EM3SuperelementId::MATCH5_BOMB:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
			break;
		case EM3SuperelementId::MATCH6_BOMB:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
			break;
		case EM3SuperelementId::MATCH7_BOMB:
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
			break;
		default:
			break;
		}
	}
}
