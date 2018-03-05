// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "Components/StaticMeshComponent.h"

const std::string k_ON_ELEMENT_STATE_CHANGED = "ON_ELEMENT_STATE_CHANGED";
const std::string k_ON_ELEMENT_ID_CHANGED = "ON_ELEMENT_ID_CHANGED";

FORWARD_DECL_STRONG(M3ElementModel)

UM3ElementViewAccessor::UM3ElementViewAccessor() {
	OnSwapEndedDelegate.BindUFunction(this, FName("OnSwapEnded"));
}

void UM3ElementViewAccessor::OnSwapEnded() {
	OnSwapEndedCallback.ExecuteIfBound();
}

UM3ElementViewDelegate::UM3ElementViewDelegate() {
}

UM3ElementViewDelegate::~UM3ElementViewDelegate() {
}

int UM3ElementViewDelegate::ViewSTTI() const {
	return static_cast<int>(M3ElementView::ClassGuid());
}

int UNIQUE_ID = 0;
M3ElementView::M3ElementView(AActor* _Superview) : M3View(_Superview) {
	std::stringstream UNIQUE_ID_STRING_STREAM;
	UNIQUE_ID_STRING_STREAM << "ELEMENT_ACCESSOR_" << UNIQUE_ID;
	std::string UNIQUE_ID_STRING = UNIQUE_ID_STRING_STREAM.str();
	Accessor = NewObject<UM3ElementViewAccessor>(_Superview, FName(UNIQUE_ID_STRING.c_str(), UNIQUE_ID_STRING.length()));
	Accessor->View = _Superview;

	Accessor->OnSwapEndedCallback.BindLambda([this]() {
		UE_LOG(LogTemp, Error, TEXT("Swap ended callback"));
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_SWAP_ENDED, GetViewModel<M3ElementModel>());
	});
}

void M3ElementView::Load(AM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3ElementView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);
	const auto& ElementModel = GetViewModel<M3ElementModel>();
	const auto& ElementEntity = ElementModel->Entity->Get();

	std::shared_ptr<M3KVSlot<EM3ElementState>> OnElementStateChangedSlot = std::make_shared<M3KVSlot<EM3ElementState>>(ElementEntity->State);
	Slots[k_ON_ELEMENT_STATE_CHANGED] = OnElementStateChangedSlot;
	OnElementStateChangedSlot->Attach([this] (EM3ElementState State) {
		switch (State) {
		case EM3ElementState::SWAPPING: {
			const auto Delegate = GetDelegate<UM3ElementViewDelegate>();
			assert(Delegate != nullptr);
			if (Delegate) {
				Delegate->OnSwap(Accessor);
			}
		}
		    break;
		default:
			break;
		}
	});

	std::shared_ptr<M3KVSlot<int>> OnElementIdChangedSlot = std::make_shared<M3KVSlot<int>>(ElementEntity->ElementId);
	Slots[k_ON_ELEMENT_ID_CHANGED] = OnElementIdChangedSlot;
	OnElementIdChangedSlot->Attach([](int Id) {
		// TODO:
	});

	const auto& CellModel = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();

	GetSuperview()->SetActorRelativeLocation(FVector(Col * 110, Row * 110, 0));

	AM3BoardAssetsBundle* BoardAssetsBundle = static_cast<AM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		switch (static_cast<EM3ElementId>(ElementEntity->ElementId->Get()))
		{
		case EM3ElementId::ELEMENT_RED: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
		}
		break;
		case EM3ElementId::ELEMENT_GREEN: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
		}
		break;
		case EM3ElementId::ELEMENT_BLUE: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
		}
		break;
		case EM3ElementId::ELEMENT_YELLOW: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
		}
		break;
		case EM3ElementId::ELEMENT_ORANGE: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
		}
		break;
		case EM3ElementId::ELEMENT_PURPLE: {
			MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
			MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
		}
		break;
		default:
			break;
		}
	}
}
