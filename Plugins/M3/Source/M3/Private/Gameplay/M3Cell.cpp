// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Cell.h"
#include "M3CellModel.h"
#include "M3CellView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

AM3Cell::AM3Cell() {
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CellRootComponent"));

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("CellActionsComponent"));
}

void AM3Cell::BeginPlay() {
	Super::BeginPlay();
}

void AM3Cell::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Cell::OnLoad(UM3AssetsBundle* Bundle) {
	CellView = std::make_shared<M3CellView>(this);
	CellView->Load(Bundle);
}

void AM3Cell::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	CellModel = std::static_pointer_cast<M3CellModel>(Model);
	CellView->BindViewModel(CellModel);
}

void AM3Cell::OnBindViewDelegate() {
}

void AM3Cell::OnBindViewAccessor() {
}

M3View_INTERFACE_SharedPtr AM3Cell::GetView() const {
	return CellView;
}

M3Model_INTERFACE_SharedPtr AM3Cell::GetModel() const {
	return CellModel;
}

