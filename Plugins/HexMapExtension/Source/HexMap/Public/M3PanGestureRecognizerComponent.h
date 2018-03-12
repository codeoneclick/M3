// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "M3PanGestureRecognizerComponent.generated.h"

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UM3PanGestureRecognizerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UM3PanGestureRecognizerComponent();

private:

	bool bIsPanned = false;
	bool bIsInterrupted = false;
	float PrevTouchLocationX = 0.f;
	float PrevTouchLocationY = 0.f;

protected:

	static bool IsFinger1Touched(UWorld* World);
	static bool IsFinger2Touched(UWorld* World);
	static bool IsFinger1Touched(UWorld* World, float& LocationX, float& LocationY);
	static bool IsFinger2Touched(UWorld* World, float& LocationX, float& LocationY);

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);
};
