// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperElement.h"
#include "M3ElementModel.h"
#include "M3SuperElementModel.h"
#include "M3SuperElementView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3InteractionComponent.h"

AM3SuperElement::AM3SuperElement() {
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SuperElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("SuperElementActionsComponent"));
	UM3InteractionComponent* InteractionComponent = CreateDefaultSubobject<UM3InteractionComponent>(TEXT("SuperElementInteractionComponent"));
	InteractionComponent->isDoubleTapEnabled = true;
}

void AM3SuperElement::BeginPlay() {
	Super::BeginPlay();
}

void AM3SuperElement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3SuperElement::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3SuperElementView>(this);
	View->Load(_Bundle);
}

void AM3SuperElement::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3SuperElementModel>(_ViewModel);
	View->BindViewModel(Model);
	ensure(Model->GetParent<M3ElementModel>() != nullptr);
	Cast<UM3InteractionComponent>(GetComponentByClass(UM3InteractionComponent::StaticClass()))->SetElementModel(Model->GetParent<M3ElementModel>());
}

void AM3SuperElement::OnBindViewDelegate() {
}

void AM3SuperElement::OnBindViewAccessor() {
}

M3View_INTERFACE_SharedPtr AM3SuperElement::GetView() const {
	return View;
}

M3Model_INTERFACE_SharedPtr AM3SuperElement::GetModel() const {
	return Model;
}

void AM3SuperElement::Dispose() {
	View = nullptr;
	Model = nullptr;
	Cast<UM3InteractionComponent>(GetComponentByClass(UM3InteractionComponent::StaticClass()))->SetElementModel(nullptr);
}