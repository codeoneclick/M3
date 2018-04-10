// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3ElementsSwapController.h"
#include "M3GestureModel.h"
#include "M3SwapModel.h"
#include "M3SharedModel.h"
#include "M3BoardStateModel.h"
#include "M3Board.h"
#include "M3View.h"
#include "M3Element.h"
#include "M3ElementModel.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "M3InteractionComponent.h"

M3ElementsSwapController::M3ElementsSwapController() {
}

M3ElementsSwapController::~M3ElementsSwapController() {
}

void M3ElementsSwapController::SetInteractionView(class AActor* _InteractionView) {
	InteractionView = _InteractionView;
}

bool M3ElementsSwapController::CanBeExecuted() const {
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	return GestureModel->GetGesturesNum() != 0;
}

void M3ElementsSwapController::Execute(float Deltatime) {
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	const auto& SwapModel = M3SharedModel::GetInstance()->GetSubmodel<M3SwapModel>();
	const auto& BoardStateModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardStateModel>();
	const auto Board = static_cast<AM3Board*>(InteractionView);

	const auto PC = Board->GetWorld()->GetFirstPlayerController();
	FHitResult Hit;

	while (GestureModel->GetGesturesNum() != 0) {
		M3Gesture CurrentGesture = GestureModel->PopGesture();

		if (CurrentGesture.Gesture == EM3Gesture::PAN_START) {
			SwapModel->Entity->Get()->SwapElementA->Set(nullptr);
			SwapModel->Entity->Get()->SwapElementB->Set(nullptr);
		}

		const FVector2D ScreenSpaceLocation(CurrentGesture.Location);
		PC->GetHitResultAtScreenPosition(ScreenSpaceLocation, PC->CurrentClickTraceChannel, false, Hit);
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetComponentByClass(UM3InteractionComponent::StaticClass())) {
			const auto InteractionComponent = Cast<UM3InteractionComponent>(HitActor->GetComponentByClass(UM3InteractionComponent::StaticClass()));
			const auto ElementModel = InteractionComponent->GetElementModel();
			ensure(ElementModel != nullptr);
			if (CurrentGesture.Gesture == EM3Gesture::DOUBLE_TAP && InteractionComponent->isDoubleTapEnabled) {
				ElementModel->SetState(EM3ElementState::MATCHING);
				BoardStateModel->IncGameTurn();
			} else {
				SwapModel->AddSwapElement(ElementModel);
				const auto SwapElementA = SwapModel->GetSwapElementA();
				const auto SwapElementB = SwapModel->GetSwapElementB();

				if (SwapElementA && SwapElementB) {

					SwapElementA->SetState(EM3ElementState::SWAPPING);
					SwapElementB->SetState(EM3ElementState::SWAPPING);
					GestureModel->SetIsInterrupted(true);

					if (SwapModel->IsPossibleToSwap()) {
						SwapModel->SwapElements(SwapElementA, SwapElementB);
					}

					SwapModel->Entity->Get()->SwapElementA->Set(nullptr);
					SwapModel->Entity->Get()->SwapElementB->Set(nullptr);
				}
			}
		}

		if (CurrentGesture.Gesture == EM3Gesture::PAN_END) {
			SwapModel->Entity->Get()->SwapElementA->Set(nullptr);
			SwapModel->Entity->Get()->SwapElementB->Set(nullptr);
		}
	}
}
