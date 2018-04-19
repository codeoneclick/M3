// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3RegularElement.h"
#include "M3ElementModel.h"
#include "M3RegularElementModel.h"
#include "M3RegularElementView.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"
#include "M3InteractionComponent.h"

AM3RegularElement::AM3RegularElement() {
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RegularElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RegularElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("RegularElementActionsComponent"));
	UM3InteractionComponent* InteractionComponent = CreateDefaultSubobject<UM3InteractionComponent>(TEXT("RegularElementInteractionComponent"));
}

AM3RegularElement::~AM3RegularElement() {
	View = nullptr;
	Model = nullptr;
}

void AM3RegularElement::BeginPlay() {
	Super::BeginPlay();
}

void AM3RegularElement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3RegularElement::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3RegularElementView>(this);
	View->Load(_Bundle);
}

void AM3RegularElement::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3RegularElementModel>(_ViewModel);
	View->BindViewModel(Model);
	ensure(Model->GetParent<M3ElementModel>() != nullptr);
	Cast<UM3InteractionComponent>(GetComponentByClass(UM3InteractionComponent::StaticClass()))->SetElementModel(Model->GetParent<M3ElementModel>());
}

void AM3RegularElement::OnBindViewDelegate() {
}

void AM3RegularElement::OnBindViewAccessor() {
}

M3View_INTERFACE_SharedPtr AM3RegularElement::GetView() const {
	return View;
}

M3Model_INTERFACE_SharedPtr AM3RegularElement::GetModel() const {
	return Model;
}

void AM3RegularElement::Dispose() {
	View = nullptr;
	Model = nullptr;
	Cast<UM3InteractionComponent>(GetComponentByClass(UM3InteractionComponent::StaticClass()))->SetElementModel(nullptr);
}