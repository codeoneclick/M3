// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "M3CoordinatingController.h"
#include "M3Utilities.h"
#include "M3CoordinatingComponent.generated.h"

FORWARD_DECL_STRONG(AM3Board)
FORWARD_DECL_STRONG(AM3AssetsBundle)
FORWARD_DECL_STRONG(AM3Scheme_INTERFACE)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UM3CoordinatingComponent : public UActorComponent, public M3CoordinatingController
{
	GENERATED_BODY()

public:	

	UM3CoordinatingComponent();

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3Board* Board;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateModels();
	void CreateControllers();
	void CreateViews(AM3AssetsBundle* AssetsBundle);
	void OnModelChanged(AM3Scheme_INTERFACE* Scheme);
};
