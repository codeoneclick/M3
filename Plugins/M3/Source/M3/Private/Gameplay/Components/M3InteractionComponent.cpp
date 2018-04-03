// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3InteractionComponent.h"

UM3InteractionComponent::UM3InteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3InteractionComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UM3InteractionComponent::SetElementModel(const M3ElementModel_SharedPtr& _ElementModel) {
	ElementModel = _ElementModel;
}

M3ElementModel_SharedPtr UM3InteractionComponent::GetElementModel() const {
	return ElementModel;
}

