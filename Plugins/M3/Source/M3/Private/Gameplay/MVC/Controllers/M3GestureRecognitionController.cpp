// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3GestureRecognitionController.h"
#include "M3TapGestureResponderComponent.h"
#include "M3PanGestureResponderComponent.h"
#include "M32TapGestureResponderComponent.h"
#include "M3Board.h"
#include "GameFramework/Actor.h"
#include "M3GestureModel.h"
#include "M3SharedModel.h"

M3GestureRecognitionController::M3GestureRecognitionController() {
}

M3GestureRecognitionController::~M3GestureRecognitionController() {
}

bool M3GestureRecognitionController::CanBeExecuted() const {
	return false;
}

void M3GestureRecognitionController::Execute(float Deltatime) {

}

void M3GestureRecognitionController::SetInteractionView(class AActor* _InteractionView) {
	InteractionView = _InteractionView;

	AM3Board* Board = static_cast<AM3Board*>(_InteractionView);
	Board->TapGestureResponderComponent->OnTapDelegate.BindRaw(this, &M3GestureRecognitionController::OnTapGesture);
	Board->PanGestureResponderComponent->OnPanDelegate.BindRaw(this, &M3GestureRecognitionController::OnPanGesture);
	Board->DoubleTapGestureResponderComponent->On2TapDelegate.BindRaw(this, &M3GestureRecognitionController::On2TapGesture);
}

void M3GestureRecognitionController::OnTapGesture(const ETouchIndex::Type FingerIndex, const FVector Location) {
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	if (!GestureModel->GetIsPanned() && !GestureModel->GetIsInterrupted()) {
		GestureModel->PushGesture(EM3Gesture::TAP, Location);
	}
}

void M3GestureRecognitionController::On2TapGesture(const ETouchIndex::Type FingerIndex, const FVector Location) {
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	if (!GestureModel->GetIsPanned() && !GestureModel->GetIsInterrupted()) {
		GestureModel->PushGesture(EM3Gesture::DOUBLE_TAP, Location);
	}
}

void M3GestureRecognitionController::OnPanGesture(EM3PanState State, const FVector Location, const FVector Delta) {
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	switch (State) {
	case EM3PanState::START:
		GestureModel->PushGesture(EM3Gesture::PAN_START, Location);
		GestureModel->SetIsPanned(true);
		GestureModel->SetIsInterrupted(false);
		break;
	case EM3PanState::MOVE:
		if (!GestureModel->GetIsInterrupted()) {
			GestureModel->PushGesture(EM3Gesture::PAN, Location);
		}
		break;
	case EM3PanState::END:
		if (!GestureModel->GetIsInterrupted()) {
			GestureModel->PushGesture(EM3Gesture::PAN_END, Location);
		}
		GestureModel->SetIsInterrupted(false);
		GestureModel->SetIsPanned(false);
		break;
	default:
		break;
	}
	
}
