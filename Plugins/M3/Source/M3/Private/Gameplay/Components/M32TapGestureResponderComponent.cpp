// Copyright serhii serhiiv 2018 All rights reserved.

#include "M32TapGestureResponderComponent.h"

UM32TapGestureResponderComponent::UM32TapGestureResponderComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM32TapGestureResponderComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM32TapGestureResponderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

