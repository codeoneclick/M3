// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"
#include "InputCoreTypes.h"
#include "M3PanGestureResponderComponent.h"

class M3_API M3GestureRecognitionController : public M3MediatingController
{
protected:

	class AActor* InteractionView = nullptr;

	void OnTapGesture(const ETouchIndex::Type FingerIndex, const FVector Location);
	void On2TapGesture(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnPanGesture(EM3PanState State, const FVector Location, const FVector Delta);

public:

	CTTI_CLASS_GUID(M3GestureRecognitionController, M3MediatingController_INTERFACE::GuidsContainer)

	M3GestureRecognitionController();
	~M3GestureRecognitionController();

	void SetInteractionView(class AActor* InteractionView);

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};
