// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ViewDelegate.h"

UM3ViewDelegate_INTERFACE::UM3ViewDelegate_INTERFACE()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3ViewDelegate_INTERFACE::BeginPlay()
{
	Super::BeginPlay();
}

void UM3ViewDelegate_INTERFACE::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UM3ViewDelegate_INTERFACE::ViewSTTI() const {
	return -1;
}