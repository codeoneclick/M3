// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SwapModel.h"
#include "M3SharedModel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "M3BoardSettingsModel.h"

const std::string k_ON_ELEMENT_STATE_CHANGED = "ON_ELEMENT_STATE_CHANGED";
const std::string k_ON_ELEMENT_ID_CHANGED = "ON_ELEMENT_ID_CHANGED";

FORWARD_DECL_STRONG(M3ElementModel)

UM3ElementViewAccessor::UM3ElementViewAccessor() {
	OnSwapEndedDelegate.BindUFunction(this, FName("OnSwapEnded"));
	OnMatchEndedDelegate.BindUFunction(this, FName("OnMatchEnded"));
	OnDropEndedDelegate.BindUFunction(this, FName("OnDropEnded"));
	OnSpawnEndedDelegate.BindUFunction(this, FName("OnSpawnEnded"));
}

void UM3ElementViewAccessor::OnSwapEnded() {
	OnSwapEndedCallback.ExecuteIfBound();
}

void UM3ElementViewAccessor::OnMatchEnded() {
	OnMatchEndedCallback.ExecuteIfBound();
}

void UM3ElementViewAccessor::OnDropEnded() {
	OnDropEndedCallback.ExecuteIfBound();
}

void UM3ElementViewAccessor::OnSpawnEnded() {
	OnSpawnEndedCallback.ExecuteIfBound();
}

UM3ElementViewDelegate::UM3ElementViewDelegate() {
}

UM3ElementViewDelegate::~UM3ElementViewDelegate() {
}

int UM3ElementViewDelegate::ViewSTTI() const {
	return static_cast<int>(M3ElementView::ClassGuid());
}

M3ElementView::M3ElementView(AActor* _Superview) : M3View(_Superview) {

}

M3ElementView::~M3ElementView() {
	Accessor->RemoveFromRoot();
}

void M3ElementView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);

	Accessor = NewObject<UM3ElementViewAccessor>();
	Accessor->AddToRoot();
	Accessor->View = Superview;

	Accessor->OnSwapEndedCallback.BindLambda([=]() {
		const auto ElementModel = GetViewModel<M3ElementModel>();
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_SWAP_ENDED, ElementModel);
	});

	Accessor->OnMatchEndedCallback.BindLambda([=]() {
		const auto ElementModel = GetViewModel<M3ElementModel>();
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_MATCH_ENDED, ElementModel);
	});

	Accessor->OnDropEndedCallback.BindLambda([=]() {
		const auto ElementModel = GetViewModel<M3ElementModel>();
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_DROP_ENDED, ElementModel);
	});

	Accessor->OnSpawnEndedCallback.BindLambda([=]() {
		const auto ElementModel = GetViewModel<M3ElementModel>();
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_SPAWN_ENDED, ElementModel);
	});
}

void M3ElementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	Accessor->ElementSize = BoardSettingsModel->GetElementSize();

	const auto& ElementModel = GetViewModel<M3ElementModel>();
	const auto& ElementEntity = ElementModel->Entity->Get();

	std::shared_ptr<M3KVSlot<EM3ElementState>> OnElementStateChangedSlot = std::make_shared<M3KVSlot<EM3ElementState>>(ElementEntity->State);
	Slots[k_ON_ELEMENT_STATE_CHANGED] = OnElementStateChangedSlot;
	OnElementStateChangedSlot->Attach([=] (EM3ElementState State) {
		const auto Delegate = GetDelegate<UM3ElementViewDelegate>();
		assert(Delegate != nullptr);

		switch (State) {
			case EM3ElementState::SWAPPING:
				if (Delegate) {
					const auto SwapModel = M3SharedModel::GetInstance()->GetSubmodel<M3SwapModel>();
					Accessor->IsPosibleToSwap = SwapModel->IsPossibleToSwap();

					Accessor->CurrentCol = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetCol();
					Accessor->CurrentRow = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetRow();

					const auto OppositeElementModel = SwapModel->GetSwapElementA() == GetViewModel<M3ElementModel>() ? SwapModel->GetSwapElementB() : SwapModel->GetSwapElementA();
					
					Accessor->OppositeCol = OppositeElementModel->GetParent<M3CellModel>()->GetCol();
					Accessor->OppositeRow = OppositeElementModel->GetParent<M3CellModel>()->GetRow();

					Delegate->OnSwap(Accessor);
				}
				break;
			case EM3ElementState::MATCHING:
				if (Delegate) {
					Accessor->CurrentCol = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetCol();
					Accessor->CurrentRow = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetRow();

					Delegate->OnMatch(Accessor);
				}
				break;
			case EM3ElementState::DROPPING:
				if (Delegate) {
					Accessor->CurrentCol = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetCol();
					Accessor->CurrentRow = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetRow();

					Delegate->OnDrop(Accessor);
				}
				break;
			case EM3ElementState::SPAWNING:
				if (Delegate) {
					Accessor->CurrentCol = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetCol();
					Accessor->CurrentRow = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetRow();

					Delegate->OnSpawn(Accessor);
				}
				break;
			default:
				break;
		}
	});

	std::shared_ptr<M3KVSlot<int>> OnElementIdChangedSlot = std::make_shared<M3KVSlot<int>>(ElementEntity->ElementId);
	Slots[k_ON_ELEMENT_ID_CHANGED] = OnElementIdChangedSlot;
	OnElementIdChangedSlot->Attach([=](int Id) {
		if (Id != -1) {
			SetElementVisual(Id);
		}
	});

	const auto& CellModel = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();

	GetSuperview()->SetActorRelativeLocation(FVector(Row * BoardSettingsModel->GetElementSize().Y, Col * BoardSettingsModel->GetElementSize().X, 0));
	SetElementVisual(ElementModel->GetElementId());
}

void M3ElementView::SetElementVisual(int ElementId) {
	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (static_cast<EM3ElementId>(ElementId))
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
