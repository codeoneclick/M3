// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3ViewDelegates_API.generated.h"

FORWARD_DECL_STRONG(UM3ViewDelegate_INTERFACE)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class HEXMAP_API AM3ViewDelegates_API : public AActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	std::vector<std::pair<TSubclassOf<UM3ViewDelegate_INTERFACE>, UM3ViewDelegate_INTERFACE*>> DelegatesLinkages;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "Delegates", EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UM3ViewDelegate_INTERFACE>> Delegates;

	TSubclassOf<UM3ViewDelegate_INTERFACE> GetDelegate(int ViewSTTI) const;
};
