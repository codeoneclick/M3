// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Superelement.h"
#include "M3SuperElementModel.h"
#include "M3SuperElementView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

AM3Superelement::AM3Superelement() {
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SuperElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("SuperElementActionsComponent"));
}

void AM3Superelement::BeginPlay() {
	Super::BeginPlay();
}

void AM3Superelement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Superelement::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3SuperelementView>(this);
	View->Load(_Bundle);
}

void AM3Superelement::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3SuperelementModel>(_ViewModel);
	View->BindViewModel(Model);
}

void AM3Superelement::OnBindViewDelegate() {

}

M3View_INTERFACE_SharedPtr AM3Superelement::GetView() const {
	return View;
}
M3Model_INTERFACE_SharedPtr AM3Superelement::GetModel() const {
	return Model;
}