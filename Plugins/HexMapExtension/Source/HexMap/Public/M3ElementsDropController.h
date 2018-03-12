// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3MediatingController.h"

FORWARD_DECL_STRONG(M3BoardModel)
FORWARD_DECL_STRONG(M3CellModel)

class HEXMAP_API M3ElementsDropController : public M3MediatingController
{
private:

	bool DropElement(const M3CellModel_SharedPtr& From, const M3CellModel_SharedPtr& To);
	bool DropUpDown(const M3BoardModel_SharedPtr& Board, const M3CellModel_SharedPtr& Cell, int Col, int Row);
	bool DropDiagonal(const M3BoardModel_SharedPtr& Board, int Col, int Row);

public:

	CTTI_CLASS_GUID(M3ElementsDropController, M3MediatingController_INTERFACE::GuidsContainer)

	M3ElementsDropController();
	~M3ElementsDropController();

	bool CanBeExecuted() const override;
	void Execute(float Deltatime) override;
};