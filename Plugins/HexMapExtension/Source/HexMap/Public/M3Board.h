// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3Board.generated.h"

FORWARD_DECL_STRONG(M3BoardModel)
FORWARD_DECL_STRONG(M3BoardView)
FORWARD_DECL_STRONG(UM3AssetsBundle)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(UM3ViewFactory)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
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

	void OnLoad(UM3ViewFactory* ViewFactory, UM3AssetsBundle* Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);
	void OnBindViewDelegate();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3TapGestureResponderComponent* TapGestureResponderComponent;

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3PanGestureResponderComponent* PanGestureResponderComponent;
};
