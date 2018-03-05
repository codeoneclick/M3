// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3Utilities.h"
#include "M3ViewDelegate.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class HEXMAP_API UM3ViewDelegate_INTERFACE : public UActorComponent {

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UM3ViewDelegate_INTERFACE();
	virtual ~UM3ViewDelegate_INTERFACE() = default;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual int ViewSTTI() const;
};
