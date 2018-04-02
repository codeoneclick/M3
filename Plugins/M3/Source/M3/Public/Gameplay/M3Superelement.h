// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Superelement.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3Superelement : public ACharacter {
	GENERATED_BODY()

public:
	AM3Superelement();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
