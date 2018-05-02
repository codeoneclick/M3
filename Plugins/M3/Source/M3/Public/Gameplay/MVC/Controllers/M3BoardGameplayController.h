// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3ElementModel)

class M3_API M3BoardGameplayController : public M3MediatingController
{
private:

	static void GeneratePotentialSwaps();
	static void DetectMatches();
	static void CreateHoles();

protected:

	void OnGameStarted();
	void OnElementSwapEnded(const M3ElementModel_SharedPtr& ElementModel, bool IsPossibleToSwap);
	void OnElementMatchEnded(const M3ElementModel_SharedPtr& ElementModel);
	void OnElementDropEnded(const M3ElementModel_SharedPtr& ElementModel);
	void OnElementSpawnEnded(const M3ElementModel_SharedPtr& ElementModel);

public:

	CTTI_CLASS_GUID(M3BoardGameplayController)

	M3BoardGameplayController();
	~M3BoardGameplayController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};

