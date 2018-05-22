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
FORWARD_DECL_STRONG(AM3Blocker)
FORWARD_DECL_STRONG(AM3SuperElement)
FORWARD_DECL_STRONG(M3KVSlot_INTERFACE)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3))
class AM3GamePlayerController : public APlayerController {
private:

	GENERATED_BODY()

protected:

	M3AppEventModelProp_SharedPtr OnMatchingEvent = nullptr;
	void OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);

	virtual void BeginPlay() override;

	std::unordered_map<std::string, M3KVSlot_INTERFACE_SharedPtr> Slots;

public:

	AM3GamePlayerController();
	~AM3GamePlayerController();

	class AM3App* GetM3App() const;
	class AM3Board* GetM3Board() const;

	virtual void Tick(float DeltaTime) override;
};


