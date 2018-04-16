// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"
#include "M3SwapModel.h"
#include "M3SharedModel.h"
#include "M3ViewActionsComponent.h"
#include "M3BoardSettingsModel.h"
#include "M3Element.h"

namespace ELEMENT_VIEW_CONNECTION {
	const std::string k_ON_ELEMENT_STATE_CHANGED = "ON_ELEMENT_STATE_CHANGED";
	const std::string k_ON_ASSIGNED_STATE_CHANGED = "ON_ASSIGNED_STATE_CHANGED";
}

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

FVector UM3ElementViewAccessor::GetCurrentLocation() {
	FVector ParentLocation = View->GetAttachParentActor()->GetActorLocation();
	return FVector(ElementSize.Y * CurrentRow + ParentLocation.X, ElementSize.X * CurrentCol + ParentLocation.Y, 0);
}

FVector UM3ElementViewAccessor::GetOppositeLocation() {
	FVector ParentLocation = View->GetAttachParentActor()->GetActorLocation();
	return FVector(ElementSize.Y * OppositeRow + ParentLocation.X, ElementSize.X * OppositeCol + ParentLocation.Y, 0);
}

UM3ElementViewDelegate::UM3ElementViewDelegate() {
}

UM3ElementViewDelegate::~UM3ElementViewDelegate() {
}

void UM3ElementViewDelegate::OnSwap_Implementation(UM3ElementViewAccessor* Accessor) {
	const auto ActionsComponent = static_cast<UM3ViewActionsComponent *>(Accessor->View->GetComponentByClass(UM3ViewActionsComponent::StaticClass()));

	ActionsComponent->RunAction(ActionsComponent->MoveToAction(0.3, Accessor->GetOppositeLocation()));
	FM3ActionNativeCallback CallbackA;
	CallbackA.BindLambda([=]() {
		if (Accessor->IsPosibleToSwap) {
			Accessor->OnSwapEndedCallback.ExecuteIfBound();
		} else {
			ActionsComponent->RunAction(ActionsComponent->MoveToAction(0.3, Accessor->GetCurrentLocation()));
			FM3ActionNativeCallback CallbackB;
			CallbackB.BindLambda([=]() {
				Accessor->OnSwapEndedCallback.ExecuteIfBound();
			});
			ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackB));
		}
	});
	ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackA));
}

void UM3ElementViewDelegate::OnMatch_Implementation(UM3ElementViewAccessor* Accessor) {
	const auto ActionsComponent = static_cast<UM3ViewActionsComponent *>(Accessor->View->GetComponentByClass(UM3ViewActionsComponent::StaticClass()));

	ActionsComponent->RunAction(ActionsComponent->ScaleToAction(0.15, FVector(1.25f)));
	FM3ActionNativeCallback CallbackA;
	CallbackA.BindLambda([=]() {
		ActionsComponent->RunAction(ActionsComponent->ScaleToAction(0.15, FVector(0.001f)));
		FM3ActionNativeCallback CallbackB;
		CallbackB.BindLambda([=]() {
			Accessor->OnMatchEndedCallback.ExecuteIfBound();
		});
		ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackB));
	});
	ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackA));
}

void UM3ElementViewDelegate::OnDrop_Implementation(UM3ElementViewAccessor* Accessor) {
	const auto ActionsComponent = static_cast<UM3ViewActionsComponent *>(Accessor->View->GetComponentByClass(UM3ViewActionsComponent::StaticClass()));

	ActionsComponent->RunAction(ActionsComponent->MoveToAction(0.3, Accessor->GetCurrentLocation()));
	FM3ActionNativeCallback CallbackA;
	CallbackA.BindLambda([=]() {
		Accessor->OnDropEndedCallback.ExecuteIfBound();
	});
	ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackA));
}

void UM3ElementViewDelegate::OnSpawn_Implementation(UM3ElementViewAccessor* Accessor) {
	const auto ActionsComponent = static_cast<UM3ViewActionsComponent *>(Accessor->View->GetComponentByClass(UM3ViewActionsComponent::StaticClass()));
	Accessor->View->SetActorScale3D(FVector(0.f));
	Accessor->View->SetActorLocation(Accessor->GetCurrentLocation());
	ActionsComponent->RunAction(ActionsComponent->ScaleToAction(0.3, FVector(1.f)));
	FM3ActionNativeCallback CallbackA;
	CallbackA.BindLambda([=]() {
		Accessor->OnSpawnEndedCallback.ExecuteIfBound();
	});
	ActionsComponent->RunAction(ActionsComponent->CallbackAction(0.01, CallbackA));
}

M3ElementView::M3ElementView(AActor* _Superview) : M3View(_Superview) {

}

M3ElementView::~M3ElementView() {
	if (Accessor && Accessor->IsRooted()) {
		Accessor->RemoveFromRoot();
	}
	Accessor = nullptr;
}

void M3ElementView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);

	Accessor = NewObject<UM3ElementViewAccessor>();
	Accessor->AddToRoot();
	Accessor->View = Superview;

	Accessor->OnSwapEndedCallback.BindLambda([=]() {
		const auto ElementModel = GetViewModel<M3ElementModel>();
		bool IsPossibleToSwap = Accessor->IsPosibleToSwap;
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr, bool>, M3ElementModel_SharedPtr, bool>(M3Events::ON_ELEMENT_SWAP_ENDED, ElementModel, IsPossibleToSwap);
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
	Slots[ELEMENT_VIEW_CONNECTION::k_ON_ELEMENT_STATE_CHANGED] = OnElementStateChangedSlot;
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
				if (Delegate && !ElementModel->IsMatchBlocked()) {
					Accessor->CurrentCol = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetCol();
					Accessor->CurrentRow = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>()->GetRow();

					Delegate->OnMatch(Accessor);
				} else if (ElementModel->IsMatchBlocked()) {
					M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<M3ElementModel_SharedPtr>, M3ElementModel_SharedPtr>(M3Events::ON_ELEMENT_MATCH_ENDED, ElementModel);
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
			case EM3ElementState::REMOVING: {

				}
				break;
			default:
				break;
		}
	});

	std::shared_ptr<M3KVSlot<bool>> OnAssignedStateChangedSlot = std::make_shared<M3KVSlot<bool>>(ElementModel->Entity->Get()->IsAssignedToView);
	Slots[ELEMENT_VIEW_CONNECTION::k_ON_ASSIGNED_STATE_CHANGED] = OnAssignedStateChangedSlot;
	OnAssignedStateChangedSlot->Attach([=](bool IsAssignedToView) {
		if (!IsAssignedToView) {
			Dispose<AM3Element>();
		}
	});

	const auto& CellModel = GetViewModel<M3ElementModel>()->GetParent<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();

	GetSuperview()->SetActorRelativeLocation(FVector(Row * BoardSettingsModel->GetElementSize().Y, Col * BoardSettingsModel->GetElementSize().X, 0));
}
