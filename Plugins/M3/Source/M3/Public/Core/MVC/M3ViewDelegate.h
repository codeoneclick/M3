// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3ViewDelegate.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class M3_API UM3ViewDelegate_INTERFACE : public UObject {

	GENERATED_BODY()

public:

	UM3ViewDelegate_INTERFACE();
	virtual ~UM3ViewDelegate_INTERFACE() = default;

	virtual void Dispose();
};
