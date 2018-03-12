// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3ElementModel)

class HEXMAP_API M3BoardGameplayController : public M3MediatingController
{
private:

	static void GeneratePotentialSwaps();
	static void DetectMatches();
	static void CreateHoles();

protected:

	void OnGameStarted();
	void OnElementSwapEnded(const M3ElementModel_SharedPtr& ElementModel);
	void OnElementMatchEnded(const M3ElementModel_SharedPtr& ElementModel);
	void OnElementDropEnded(const M3ElementModel_SharedPtr& ElementModel);

public:

	CTTI_CLASS_GUID(M3BoardGameplayController, M3MediatingController_INTERFACE::GuidsContainer)

	M3BoardGameplayController();
	~M3BoardGameplayController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};

