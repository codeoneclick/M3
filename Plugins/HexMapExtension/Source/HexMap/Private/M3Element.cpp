// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Element.h"
#include "M3ElementModel.h"
#include "M3ElementView.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"

AM3Element::AM3Element()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ElementView = std::make_shared<M3ElementView>(this);
}

void AM3Element::BeginPlay()
{
	Super::BeginPlay();
}

void AM3Element::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AM3Element::OnLoad(AM3AssetsBundle* Bundle) {
	ElementView->Load(Bundle);
}

void AM3Element::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	ElementView->BindViewModel(ElementModel);
}

M3View_INTERFACE_SharedPtr AM3Element::GetView() const {
	return ElementView;
}

M3Model_INTERFACE_SharedPtr AM3Element::GetModel() const {
	return ElementModel;
}

