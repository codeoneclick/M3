// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3PanGestureResponderComponent.generated.h"

UENUM(BlueprintType)
enum class EM3PanState : uint8 {
	START UMETA(DisplayName = "Start"),
	MOVE UMETA(DisplayName = "Move"),
	END UMETA(DisplayName = "End"),
};

DECLARE_DELEGATE_ThreeParams(FM3OnPanDelegateSignature, EM3PanState, const FVector, const FVector)

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class M3_API UM3PanGestureResponderComponent : public UActorComponent
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
