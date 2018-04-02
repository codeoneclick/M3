// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3TapGestureResponderComponent.h"

UM3TapGestureResponderComponent::UM3TapGestureResponderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3TapGestureResponderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UM3TapGestureResponderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

