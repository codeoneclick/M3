// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Element.h"
#include "M3ElementModel.h"
#include "M3ElementView.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

AM3Element::AM3Element() {
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("ElementActionsComponent"));
}

AM3Element::~AM3Element() {
	View = nullptr;
	Model = nullptr;
}

void AM3Element::BeginPlay() {
	Super::BeginPlay();
}

void AM3Element::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Element::OnLoad(UM3AssetsBundle* _Bundle) {
	View = std::make_shared<M3ElementView>(this);
	View->Load(_Bundle);
}

void AM3Element::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	Model = std::static_pointer_cast<M3ElementModel>(_ViewModel);
	View->BindViewModel(Model);
}

void AM3Element::OnBindViewDelegate() {
	if (Delegate_BP) {
		Delegate = NewObject<UM3ViewDelegate_INTERFACE>(this, Delegate_BP);
	} else {
		Delegate = NewObject<UM3ElementViewDelegate>();
	}
	View->BindViewDelegate(Delegate);
}

void AM3Element::OnBindViewAccessor() {
	if (Accessor_BP) {
		Accessor = NewObject<UM3ElementViewAccessor>(this, Accessor_BP);
	}
	else {
		Accessor = NewObject<UM3ElementViewAccessor>();
	}
	View->BindViewAccessor(Accessor);
}

M3View_INTERFACE_SharedPtr AM3Element::GetView() const {
	return View;
}

M3Model_INTERFACE_SharedPtr AM3Element::GetModel() const {
	return Model;
}

void AM3Element::Dispose() {
	View = nullptr;
	Model = nullptr;
}

