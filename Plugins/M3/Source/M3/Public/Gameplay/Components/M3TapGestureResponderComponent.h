// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "M3TapGestureResponderComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FM3OnTapDelegateSignature, const ETouchIndex::Type, const FVector)

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class M3_API UM3TapGestureResponderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UM3TapGestureResponderComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FM3OnTapDelegateSignature OnTapDelegate;
};
