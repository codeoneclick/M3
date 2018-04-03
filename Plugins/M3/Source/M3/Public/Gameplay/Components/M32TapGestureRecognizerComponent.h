// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "M32TapGestureRecognizerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class M3_API UM32TapGestureRecognizerComponent : public UActorComponent {
	GENERATED_BODY()

public:	

	UM32TapGestureRecognizerComponent();

protected:

	virtual void BeginPlay() override;

	float CurrentTime = 0;
	float LastTapTime = 0;

public:	

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadWrite)
	float TimeInterval = 1.f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);
};
