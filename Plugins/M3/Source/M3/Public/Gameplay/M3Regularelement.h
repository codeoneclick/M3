// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Element.h"
#include "M3Regularelement.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3Regularelement : public AM3Element {

	GENERATED_BODY()

public:

	AM3Regularelement();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnLoad(UM3AssetsBundle* Bundle) override;
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) override;
};
