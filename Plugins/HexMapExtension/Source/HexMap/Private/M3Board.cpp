// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Board.h"
#include "M3BoardModel.h"
#include "M3BoardView.h"
#include "M3Cell.h"
#include "M3Element.h"

AM3Board::AM3Board() {
	PrimaryActorTick.bCanEverTick = true;
	BoardView = std::make_shared<M3BoardView>(this);
}

void AM3Board::BeginPlay() {
	Super::BeginPlay();
}

void AM3Board::OnLoad(AM3AssetsBundle* Bundle) {
	BoardView->Load(Bundle);
}

void AM3Board::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	BoardModel = std::static_pointer_cast<M3BoardModel>(Model);
	BoardView->BindViewModel(BoardModel);
}

void AM3Board::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

M3View_INTERFACE_SharedPtr AM3Board::GetView() const {
	return BoardView;
}

M3Model_INTERFACE_SharedPtr AM3Board::GetModel() const {
	return BoardModel;
}

