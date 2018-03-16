// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Board.h"
#include "M3BoardModel.h"
#include "M3BoardView.h"
#include "M3Cell.h"
#include "M3Element.h"
#include "M3TapGestureResponderComponent.h"
#include "M3PanGestureResponderComponent.h"

AM3Board::AM3Board() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BoardRootComponent"));
	TapGestureResponderComponent = CreateDefaultSubobject<UM3TapGestureResponderComponent>(TEXT("TapGestureResponderComponent"));
	PanGestureResponderComponent = CreateDefaultSubobject<UM3PanGestureResponderComponent>(TEXT("PanGestureResponderComponent"));
}

void AM3Board::BeginPlay() {
	Super::BeginPlay();

	TapGestureResponderComponent = FindComponentByClass<UM3TapGestureResponderComponent>();
	if (!TapGestureResponderComponent) {
		UE_LOG(LogTemp, Error, TEXT("Can't find TapGestureResponderComponent"));
	}

	PanGestureResponderComponent = FindComponentByClass<UM3PanGestureResponderComponent>();
	if (!PanGestureResponderComponent) {
		UE_LOG(LogTemp, Error, TEXT("Can't find PanGestureResponderComponent"));
	}
}

void AM3Board::OnLoad(UM3AssetsBundle* Bundle) {
	BoardView = std::make_shared<M3BoardView>(this);
	BoardView->Load(Bundle);
}

void AM3Board::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	BoardModel = std::static_pointer_cast<M3BoardModel>(Model);
	BoardView->BindViewModel(BoardModel);
}

void AM3Board::OnBindViewDelegate() {
	BoardView->BindViewDelegate(nullptr);
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

