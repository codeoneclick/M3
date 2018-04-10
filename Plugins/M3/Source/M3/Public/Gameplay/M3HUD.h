// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Scheme.h"
#include "GameFramework/HUD.h"
#include "M3HUD.generated.h"

FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)
FORWARD_DECL_STRONG(M3KVSlot_INTERFACE)
FORWARD_DECL_STRONG(M3AppEventModelProp)

UCLASS()
class M3_API AM3HUD : public AHUD {
	GENERATED_BODY()

private:

	M3AppEvent_INTERFACE_SharedPtr OnGameStartedEvent = nullptr;
	M3AppEventModelProp_SharedPtr OnGoalQuantityChangedEvent = nullptr;
	std::unordered_map<std::string, M3KVSlot_INTERFACE_SharedPtr> Slots;

protected:

	virtual void BeginPlay() override;
	void OnGameStarted();

	void OnGoalQuantityChanged(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);

public:

	AM3HUD();
	~AM3HUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "Delegates")
	void OnDurationChanged(int CurrentDuration, int MaxDuration, bool IsTurnBased);

	UFUNCTION(BlueprintImplementableEvent, Category = "Delegates")
	void OnScoresChanged(int CurrentScores, int Star1Scores, int Star2Scores, int Star3Scores);

	UFUNCTION(BlueprintImplementableEvent, Category = "Delegates")
	void OnGoalChanged(EM3GoalId Id, int Index, int CurrentQuantity, int MaxQuantity);

};
