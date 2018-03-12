// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3Element.generated.h"

FORWARD_DECL_STRONG(M3ElementModel)
FORWARD_DECL_STRONG(M3ElementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)
FORWARD_DECL_STRONG(UM3ViewFactory)
FORWARD_DECL_STRONG(UM3ViewDelegate_INTERFACE)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class HEXMAP_API AM3Element : public ACharacter
{
	GENERATED_BODY()
	
public:	

	AM3Element();

protected:

	M3ElementModel_SharedPtr ElementModel = nullptr;
	M3ElementView_SharedPtr ElementView = nullptr;

	virtual void BeginPlay() override;

public:	

	UPROPERTY(Category = "Delegates", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3ViewDelegate_INTERFACE> Delegate_BP;

	UPROPERTY(Category = "Delegates", EditAnywhere, BlueprintReadWrite)
	UM3ViewDelegate_INTERFACE* Delegate;

	virtual void Tick(float DeltaTime) override;

	void OnLoad(UM3ViewFactory* ViewFactory, UM3AssetsBundle* Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);
	void OnBindViewDelegate();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;
};
