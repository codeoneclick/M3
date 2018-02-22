// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CoordinatingComponent.h"
#include "Engine/World.h"
#include "M3Board.h"
#include "M3BoardModel.h"
#include "M3BoardView.h"
#include "M3SharedModel.h"

UM3CoordinatingComponent::UM3CoordinatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3CoordinatingComponent::BeginPlay()
{
	Super::BeginPlay();
	Board = GetWorld()->SpawnActor<AM3Board>(FVector(0, 0, 0), FRotator(0, 0, 0));
}

void UM3CoordinatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnUpdate(DeltaTime);
}

void UM3CoordinatingComponent::CreateModels() {
	M3BoardModel::Register();
	M3SharedModel::GetInstance()->AddSubmodel(std::make_shared<M3BoardModel>());
}

void UM3CoordinatingComponent::CreateControllers() {

}

void UM3CoordinatingComponent::CreateViews(AM3AssetsBundle* AssetsBundle) {
	Board->OnLoad(AssetsBundle);
	Board->OnBindViewModel(M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>());
}

void UM3CoordinatingComponent::OnModelChanged(AM3Scheme_INTERFACE* Scheme) {
	M3SharedModel::GetInstance()->Deserialize(Scheme);
}

