// Fill out your copyright notice in the Description page of Project Settings.

#include "M3RegularelementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3RegularelementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SharedModel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3BoardSettingsModel.h"

const std::string k_ON_ELEMENT_ID_CHANGED = "ON_ELEMENT_ID_CHANGED";

M3RegularelementView::M3RegularelementView(AActor* _Superview) : M3ElementView(_Superview) {
}

M3RegularelementView::~M3RegularelementView() {
}

void M3RegularelementView::Load(UM3AssetsBundle* _Bundle) {
	M3ElementView::Load(_Bundle);
}

void M3RegularelementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3ElementView::BindViewModel(_ViewModel);

	const auto RegularelementModel = GetViewModel<M3ElementModel>()->GetSubmodel<M3RegularelementModel>();
	std::shared_ptr<M3KVSlot<EM3ElementId>> OnElementIdChangedSlot = std::make_shared<M3KVSlot<EM3ElementId>>(RegularelementModel->Entity->Get()->Id);
	Slots[k_ON_ELEMENT_ID_CHANGED] = OnElementIdChangedSlot;
	OnElementIdChangedSlot->Attach([=](EM3ElementId Id) {
		if (Id != EM3ElementId::UNKNOWN) {
			SetElementVisual(Id);
		}
	});
	SetElementVisual(RegularelementModel->Entity->Get()->Id->Get());
}

void M3RegularelementView::SetElementVisual(EM3ElementId Id) {
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
