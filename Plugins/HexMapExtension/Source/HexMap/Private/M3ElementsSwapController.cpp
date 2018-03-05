// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementsSwapController.h"
#include "M3GestureModel.h"
#include "M3SwapModel.h"
#include "M3SharedModel.h"
#include "M3Board.h"
#include "M3View.h"
#include "M3ElementView.h"
#include "M3ElementModel.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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
	const auto Board = static_cast<AM3Board*>(InteractionView);
	const auto BoardView = Board->GetView();
	const auto ElementViews = BoardView->GetSubviews<M3ElementView>();

	const auto PC = Board->GetWorld()->GetFirstPlayerController();
	FHitResult Hit;

	while (GestureModel->GetGesturesNum() != 0) {
		M3Gesture CurrentGesture = GestureModel->PopGesture();
		const FVector2D ScreenSpaceLocation(CurrentGesture.Location);
		PC->GetHitResultAtScreenPosition(ScreenSpaceLocation, PC->CurrentClickTraceChannel, false, Hit);
		AActor* HitActor = Hit.GetActor();
		if (HitActor) {
			for (const auto& ElementView : ElementViews) {
				if (ElementView->GetSuperview() == HitActor) {
					UE_LOG(LogTemp, Error, TEXT("Hit Element"));
					ElementView->GetViewModel<M3ElementModel>()->Entity->Get()->State->Set(EM3ElementState::SWAPPING);
					break;
				}
			}
		}
	}
}
