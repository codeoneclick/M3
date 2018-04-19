// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "M3Utilities.h"
#include "M3GamePlayerController.generated.h"

FORWARD_DECL_STRONG(M3AppEventModelProp)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)
FORWARD_DECL_STRONG(UM3ElementExplosionViewDelegate)
FORWARD_DECL_STRONG(UM3ElementExplosionViewAccessor)
FORWARD_DECL_STRONG(AM3Element)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3))
class AM3GamePlayerController : public APlayerController {
private:

	GENERATED_BODY()

protected:

	M3AppEventModelProp_SharedPtr OnMatchingEvent;
	void OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);

	virtual void BeginPlay() override;

public:

	AM3GamePlayerController();
	~AM3GamePlayerController();

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Element> ElementExplosion_BP;

	UPROPERTY(Category = "Delegates", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3ElementExplosionViewDelegate> ElementExplosionViewDelegate_BP;

	UPROPERTY(Category = "Accessors", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3ElementExplosionViewAccessor> ElementExplosionViewAccessor_BP;
};


