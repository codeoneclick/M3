// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3ViewAccessor.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Accessors))
class M3_API UM3ViewAccessor_INTERFACE : public UObject {

	GENERATED_BODY()

public:

	UM3ViewAccessor_INTERFACE();
	virtual ~UM3ViewAccessor_INTERFACE() = default;

	virtual void Dispose();
};
