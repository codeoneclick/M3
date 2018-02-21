// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Cell.h"
#include "M3CellModel.h"
#include "M3CellView.h"

AM3Cell::AM3Cell()
{
	PrimaryActorTick.bCanEverTick = true;
	CellView = std::make_shared<M3CellView>(this);
}

void AM3Cell::BeginPlay()
{
	Super::BeginPlay();
}

void AM3Cell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AM3Cell::OnLoad(AM3AssetsBundle* Bundle) {
	CellView->Load(Bundle);
}

void AM3Cell::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	CellModel = std::static_pointer_cast<M3CellModel>(Model);
	CellView->BindViewModel(CellModel);
}

M3View_INTERFACE_SharedPtr AM3Cell::GetView() const {
	return CellView;
}

M3Model_INTERFACE_SharedPtr AM3Cell::GetModel() const {
	return CellModel;
}
