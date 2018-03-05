// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3Utilities.h"
#include "M3BoardGeneratorComponent.generated.h"

FORWARD_DECL_STRONG(AM3App)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UM3BoardGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UM3BoardGeneratorComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Generate(AM3App* App);
};
