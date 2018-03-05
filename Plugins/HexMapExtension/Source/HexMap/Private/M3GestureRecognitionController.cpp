// Fill out your copyright notice in the Description page of Project Settings.

#include "M3GestureRecognitionController.h"
#include "M3TapGestureResponderComponent.h"
#include "M3PanGestureResponderComponent.h"
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
}

void M3GestureRecognitionController::OnTapGesture(const ETouchIndex::Type FingerIndex, const FVector Location) {
	UE_LOG(LogTemp, Error, TEXT("Tap"));
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	GestureModel->PushGesture(EM3Gesture::TAP, Location);
}

void M3GestureRecognitionController::OnPanGesture(const FVector Location, const FVector Delta) {
	UE_LOG(LogTemp, Error, TEXT("Pan"));
	const auto& GestureModel = M3SharedModel::GetInstance()->GetSubmodel<M3GestureModel>();
	GestureModel->PushGesture(EM3Gesture::PAN, Location);
}
