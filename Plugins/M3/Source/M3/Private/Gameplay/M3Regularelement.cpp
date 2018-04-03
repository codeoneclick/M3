// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Regularelement.h"
#include "M3RegularelementModel.h"
#include "M3RegularelementView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

AM3Regularelement::AM3Regularelement() {
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RegularElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RegularElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("RegularElementActionsComponent"));
}

AM3Regularelement::~AM3Regularelement() {
	View = nullptr;
	Model = nullptr;
}

void AM3Regularelement::BeginPlay() {
	Super::BeginPlay();
}

void AM3Regularelement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Regularelement::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3RegularelementView>(this);
	View->Load(_Bundle);
}

void AM3Regularelement::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3RegularelementModel>(_ViewModel);
	View->BindViewModel(Model);
}

void AM3Regularelement::OnBindViewDelegate() {

}

M3View_INTERFACE_SharedPtr AM3Regularelement::GetView() const {
	return View;
}

M3Model_INTERFACE_SharedPtr AM3Regularelement::GetModel() const {
	return Model;
}

void AM3Regularelement::Dispose() {
	View = nullptr;
	Model = nullptr;
}