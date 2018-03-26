// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Regularelement.h"
#include "M3ElementModel.h"
#include "M3RegularelementView.h"

AM3Regularelement::AM3Regularelement() {
	PrimaryActorTick.bCanEverTick = true;
}

void AM3Regularelement::BeginPlay() {
	Super::BeginPlay();
}

void AM3Regularelement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Regularelement::OnLoad(UM3AssetsBundle* _Bundle) {
	ElementView = std::make_shared<M3RegularelementView>(this);
	ElementView->Load(_Bundle);
}

void AM3Regularelement::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	ElementView->BindViewModel(ElementModel);
}