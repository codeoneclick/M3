// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3Board.generated.h"


FORWARD_DECL_STRONG(M3BoardModel)
FORWARD_DECL_STRONG(M3BoardView)
FORWARD_DECL_STRONG(AM3AssetsBundle)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)

UCLASS()
class HEXMAP_API AM3Board : public AActor
{
	GENERATED_BODY()
	
public:	

	AM3Board();

protected:

	M3BoardModel_SharedPtr BoardModel = nullptr;
	M3BoardView_SharedPtr BoardView = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnLoad(AM3AssetsBundle* Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;
};
