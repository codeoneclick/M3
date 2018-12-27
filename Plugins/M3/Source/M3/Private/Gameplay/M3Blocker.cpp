// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Blocker.h"
#include "M3ElementModel.h"
#include "M3BlockerModel.h"
#include "M3BlockerView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

AM3Blocker::AM3Blocker() {
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BlockerRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("BlockerActionsComponent"));
}

void AM3Blocker::BeginPlay() {
	Super::BeginPlay();
}

void AM3Blocker::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Blocker::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3BlockerView>(this);
	View->Load(_Bundle);
}

void AM3Blocker::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3BlockerModel>(_ViewModel);
	View->BindViewModel(Model);
	ensure(Model->GetParent<M3ElementModel>() != nullptr);
}

void AM3Blocker::OnBindViewDelegate() {
}

void AM3Blocker::OnBindViewAccessor() {
}

M3View_INTERFACE_SharedPtr AM3Blocker::GetView() const {
	return View;
}

M3Model_INTERFACE_SharedPtr AM3Blocker::GetModel() const {
	return Model;
}

void AM3Blocker::Dispose() {
	View = nullptr;
	Model = nullptr;
}
