// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3PanGestureResponderComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FM3OnPanDelegateSignature, const FVector, const FVector)

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UM3PanGestureResponderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UM3PanGestureResponderComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FM3OnPanDelegateSignature OnPanDelegate;
};
