// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3Element.generated.h"

FORWARD_DECL_STRONG(M3ElementModel)
FORWARD_DECL_STRONG(M3ElementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(AM3AssetsBundle)

UCLASS()
class HEXMAP_API AM3Element : public AActor
{
	GENERATED_BODY()
	
public:	

	AM3Element();

protected:

	M3ElementModel_SharedPtr ElementModel = nullptr;
	M3ElementView_SharedPtr ElementView = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnLoad(AM3AssetsBundle* Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;
};
