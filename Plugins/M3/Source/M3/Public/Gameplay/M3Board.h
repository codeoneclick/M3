// Copyright serhii serhiiv 2018 All rights reserved.

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

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3View))
class M3_API AM3Board : public AActor {
	GENERATED_BODY()
	
public:	

	AM3Board();

protected:

	M3BoardModel_SharedPtr BoardModel = nullptr;
	M3BoardView_SharedPtr BoardView = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void OnLoad(UM3AssetsBundle* Bundle);
	virtual void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);
	virtual void OnBindViewDelegate();
	virtual void OnBindViewAccessor();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UM3TapGestureResponderComponent* TapGestureResponderComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UM3PanGestureResponderComponent* PanGestureResponderComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UM32TapGestureResponderComponent* DoubleTapGestureResponderComponent;
};
