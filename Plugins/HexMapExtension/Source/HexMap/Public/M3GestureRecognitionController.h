// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"
#include "InputCoreTypes.h"

class HEXMAP_API M3GestureRecognitionController : public M3MediatingController
{
protected:

	class AActor* InteractionView = nullptr;

	void OnTapGesture(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnPanGesture(const FVector Location, const FVector Delta);

public:

	CTTI_CLASS_GUID(M3GestureRecognitionController, M3MediatingController_INTERFACE::GuidsContainer)

	M3GestureRecognitionController();
	~M3GestureRecognitionController();

	void SetInteractionView(class AActor* InteractionView);

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
