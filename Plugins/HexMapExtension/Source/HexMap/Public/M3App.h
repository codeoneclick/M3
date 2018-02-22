// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3App.generated.h"

FORWARD_DECL_STRONG(UM3CoordinatingComponent)
FORWARD_DECL_STRONG(AM3BoardAssetsBundle)
FORWARD_DECL_STRONG(AM3BoardScheme)

UCLASS()
class HEXMAP_API AM3App : public AActor
{
	GENERATED_BODY()
	
public:	

	AM3App();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	UM3CoordinatingComponent* CoordinatingComponent;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3BoardAssetsBundle* AssetsBundle;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3BoardScheme* Scheme;
};
