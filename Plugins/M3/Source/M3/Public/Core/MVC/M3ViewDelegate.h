// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3Utilities.h"
#include "M3ViewDelegate.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class M3_API UM3ViewDelegate_INTERFACE : public UObject {

	GENERATED_BODY()

protected:

public:

	UM3ViewDelegate_INTERFACE();
	virtual ~UM3ViewDelegate_INTERFACE() = default;
};
