// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "M3TapGestureRecognizerComponent.generated.h"

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UM3TapGestureRecognizerComponent : public UActorComponent
{
GENERATED_BODY()

public:	

	UM3TapGestureRecognizerComponent();

protected:

	bool bIsInterrupted = false;
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);
};
