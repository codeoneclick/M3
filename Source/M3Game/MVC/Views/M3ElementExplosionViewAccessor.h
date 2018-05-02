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

	UPROPERTY(Category = "Accessors", VisibleAnywhere, BlueprintReadOnly)
	int ExplosionCol = -1;

	UPROPERTY(Category = "Accessors", VisibleAnywhere, BlueprintReadOnly)
	int ExplosionRow = -1;

	UPROPERTY(Category = "Accessors", VisibleAnywhere, BlueprintReadOnly)
	bool IsExplosion = false;

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FVector GetExplosionLocation();

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FVector GetExplosionIteration1Location();

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FVector GetExplosionIteration2Location();
};
