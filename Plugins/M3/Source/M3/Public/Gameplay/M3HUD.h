// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/HUD.h"
#include "M3HUD.generated.h"

FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)
FORWARD_DECL_STRONG(M3KVSlot_INTERFACE)

UCLASS()
class M3_API AM3HUD : public AHUD
{
	GENERATED_BODY()

private:

	M3AppEvent_INTERFACE_SharedPtr OnGameStartedEvent = nullptr;
	std::unordered_map<std::string, M3KVSlot_INTERFACE_SharedPtr> Slots;

protected:

	virtual void BeginPlay() override;
	void OnGameStarted();

public:

	AM3HUD();
	~AM3HUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnDurationChanged(int CurrentDuration, int MaxDuration, bool IsTurnBased);

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnScoresChanged(int CurrentScores, int Star1Scores, int Star2Scores, int Star3Scores);

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnGoalChanged(int Id, int CurrentQuantity, int MaxQuantity);

};
