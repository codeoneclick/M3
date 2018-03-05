// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CoordinatingComponent.h"
#include "Engine/World.h"
#include "M3Board.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3BoardView.h"
#include "M3GestureModel.h"
#include "M3SwapModel.h"
#include "M3BoardActionsAccumulationModel.h"
#include "M3SharedModel.h"
#include "M3GestureRecognitionController.h"
#include "M3ElementsSwapController.h"
#include "M3BoardGameplayController.h"
#include "M3BoardActionsAccumController.h"

UM3CoordinatingComponent::UM3CoordinatingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

UM3CoordinatingComponent::~UM3CoordinatingComponent() {
	M3BoardModel::Unregister();
	M3CellModel::Unregister();
	M3ElementModel::Unregister();
	M3GestureModel::Unregister();
	M3SwapModel::Unregister();
}

void UM3CoordinatingComponent::BeginPlay()
{
	Super::BeginPlay();
	Board = GetWorld()->SpawnActor<AM3Board>(FVector(0, 0, 0), FRotator(0, 0, 0));
}

void UM3CoordinatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnUpdate(DeltaTime);
}

void UM3CoordinatingComponent::CreateModels() {

	M3BoardModel::Register();
	M3CellModel::Register();
	M3ElementModel::Register();
	M3GestureModel::Register();
	M3SwapModel::Register();
	M3BoardActionsAccumulationModel::Register();

	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3GestureModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3SwapModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardActionsAccumulationModel>());
}

void UM3CoordinatingComponent::CreateControllers() {
	AddController(std::make_shared<M3GestureRecognitionController>());
	AddController(std::make_shared<M3ElementsSwapController>());
	AddController(std::make_shared<M3BoardGameplayController>());
	AddController(std::make_shared<M3BoardActionsAccumController>());
}

void UM3CoordinatingComponent::CreateViews(AM3AssetsBundle* AssetsBundle, AM3ViewDelegates_API* Delegates_API) {
	Board->OnLoad(AssetsBundle);
	Board->OnBindViewModel(M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>());
	Board->OnBindViewDelegates(Delegates_API);
}

void UM3CoordinatingComponent::OnModelChanged(AM3Scheme_INTERFACE* Scheme) {
	M3SharedModel::GetInstance()->Deserialize(Scheme);
}

void UM3CoordinatingComponent::OnStart() {
	GetControllerAs<M3GestureRecognitionController>()->SetInteractionView(Board);
	GetControllerAs<M3ElementsSwapController>()->SetInteractionView(Board);
	M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<int>, int>(M3Events::ON_GAME_STARTED, 0);
}

