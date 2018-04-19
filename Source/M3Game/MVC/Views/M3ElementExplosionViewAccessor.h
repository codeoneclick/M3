// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3ElementView.h"
#include "M3ElementExplosionViewAccessor.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Accessors))
class M3GAME_API UM3ElementExplosionViewAccessor : public UM3ElementViewAccessor {
private:

	GENERATED_BODY()

public:

	UM3ElementExplosionViewAccessor();
	~UM3ElementExplosionViewAccessor();
};
