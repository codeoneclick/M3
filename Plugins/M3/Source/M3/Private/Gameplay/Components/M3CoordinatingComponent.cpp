// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3CoordinatingComponent.h"
#include "Engine/World.h"
#include "M3AssetsBundle.h"
#include "M3Board.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3ChainModel.h"
#include "M3BoardView.h"
#include "M3GestureModel.h"
#include "M3SwapModel.h"
#include "M3BoardStateModel.h"
#include "M3BoardActionsAccumulationModel.h"
#include "M3BoardSettingsModel.h"
#include "M3GoalsModel.h"
#include "M3RegularElementModel.h"
#include "M3SuperElementModel.h"
#include "M3SpawnModel.h"
#include "M3SharedModel.h"
#include "M3AutobotModel.h"
#include "M3BlockerModel.h"
#include "M3GestureRecognitionController.h"
#include "M3ElementsSwapController.h"
#include "M3BoardGameplayController.h"
#include "M3BoardActionsAccumController.h"
#include "M3ElementsMatchController.h"
#include "M3ElementsDropController.h"
#include "M3ElementsSpawnController.h"
#include "M3GoalsController.h"
#include "M3GameStateController.h"
#include "M3SuperElementsController.h"
#include "M3AutobotController.h"
#include "M3BlockersController.h"

UM3CoordinatingComponent::UM3CoordinatingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

UM3CoordinatingComponent::~UM3CoordinatingComponent() {
	M3BoardModel::Unregister();
	M3CellModel::Unregister();
	M3ElementModel::Unregister();
	M3GestureModel::Unregister();
	M3SwapModel::Unregister();
	M3BoardActionsAccumulationModel::Unregister();
	M3ChainModel::Unregister();
	M3BoardStateModel::Unregister();
	M3BoardSettingsModel::Unregister();
	M3GoalModel::Unregister();
	M3GoalsModel::Unregister();
	M3RegularElementModel::Unregister();
	M3SuperElementModel::Unregister();
	M3BlockerModel::Unregister();
	M3SpawnModel::Unregister();
	M3AutobotModel::Unregister();

	M3SharedModel::GetInstance()->RemoveAllSubmodels();
}

void UM3CoordinatingComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3CoordinatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnUpdate(DeltaTime);
}

void UM3CoordinatingComponent::CreateModels() {

	M3BoardSettingsModel::Register();
	M3BoardModel::Register();
	M3CellModel::Register();
	M3ElementModel::Register();
	M3GestureModel::Register();
	M3SwapModel::Register();
	M3BoardActionsAccumulationModel::Register();
	M3ChainModel::Register();
	M3BoardStateModel::Register();
	M3GoalModel::Register();
	M3GoalsModel::Register();
	M3RegularElementModel::Register();
	M3SuperElementModel::Register();
	M3BlockerModel::Register();
	M3SpawnModel::Register();
	M3AutobotModel::Register();

	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardSettingsModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3GestureModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3SwapModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardActionsAccumulationModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3ChainModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardStateModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3GoalsModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3SpawnModel>());
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3AutobotModel>());
}

void UM3CoordinatingComponent::CreateControllers() {
	AddController(std::make_shared<M3GestureRecognitionController>());
	AddController(std::make_shared<M3ElementsSwapController>());
	AddController(std::make_shared<M3BoardGameplayController>());
	AddController(std::make_shared<M3BoardActionsAccumController>());
	AddController(std::make_shared<M3ElementsMatchController>());
	AddController(std::make_shared<M3ElementsSpawnController>());
	AddController(std::make_shared<M3ElementsDropController>());
	AddController(std::make_shared<M3GoalsController>());
	AddController(std::make_shared<M3SuperElementsController>());
	AddController(std::make_shared<M3GameStateController>());
	AddController(std::make_shared<M3AutobotController>());
	AddController(std::make_shared<M3BlockersController>());
}

void UM3CoordinatingComponent::CreateViews(UM3AssetsBundle* _Bundle) {
	Board = static_cast<UM3BoardAssetsBundle*>(_Bundle)->ConstructBoard(GetWorld());
	Board->OnLoad(_Bundle);
	Board->OnBindViewModel(M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>());
	Board->OnBindViewDelegate();
}

void UM3CoordinatingComponent::OnModelChanged(AM3Scheme_INTERFACE* Scheme) {
	M3SharedModel::GetInstance()->Deserialize(Scheme);
}

void UM3CoordinatingComponent::OnStart() {
	GetControllerAs<M3GestureRecognitionController>()->SetInteractionView(Board);
	GetControllerAs<M3ElementsSwapController>()->SetInteractionView(Board);
	M3GlobalDispatcher::GetInstance()->Publish<M3AppEvent<int>, int>(M3Events::ON_GAME_STARTED, 0);
}

