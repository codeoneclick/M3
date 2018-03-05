// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "M3TapGestureResponderComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FM3OnTapDelegateSignature, const ETouchIndex::Type, const FVector)

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEXMAP_API UM3TapGestureResponderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UM3TapGestureResponderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FM3OnTapDelegateSignature OnTapDelegate;
};