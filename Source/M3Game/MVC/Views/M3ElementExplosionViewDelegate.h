// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3ElementView.h"
#include "M3ElementExplosionViewDelegate.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Accessors))
class M3GAME_API UM3ElementExplosionViewDelegate : public UM3ElementViewDelegate {
private:

	GENERATED_BODY()

public:

	UM3ElementExplosionViewDelegate();
	~UM3ElementExplosionViewDelegate();
};
