// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3PanGestureResponderComponent.h"

UM3PanGestureResponderComponent::UM3PanGestureResponderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3PanGestureResponderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UM3PanGestureResponderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

