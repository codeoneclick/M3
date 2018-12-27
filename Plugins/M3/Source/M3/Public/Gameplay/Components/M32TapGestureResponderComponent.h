// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "M32TapGestureResponderComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FM3On2TapDelegateSignature, const ETouchIndex::Type, const FVector)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class M3_API UM32TapGestureResponderComponent : public UActorComponent {
	GENERATED_BODY()

public:	

	UM32TapGestureResponderComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FM3On2TapDelegateSignature On2TapDelegate;
};
