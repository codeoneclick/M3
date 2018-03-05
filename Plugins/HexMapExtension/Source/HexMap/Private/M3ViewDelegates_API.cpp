// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ViewDelegates_API.h"
#include "M3ViewDelegate.h"
#include "Engine/World.h"

void AM3ViewDelegates_API::PostInitializeComponents() {
	Super::PostInitializeComponents();
	
	for (const auto Delegate : Delegates) {
		DelegatesLinkages.push_back(std::make_pair(Delegate, NewObject<UM3ViewDelegate_INTERFACE>(this, Delegate)));
	}
}

void AM3ViewDelegates_API::BeginPlay() {
	Super::BeginPlay();
}

void AM3ViewDelegates_API::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

TSubclassOf<UM3ViewDelegate_INTERFACE> AM3ViewDelegates_API::GetDelegate(int ViewSTTI) const {
	TSubclassOf<UM3ViewDelegate_INTERFACE> Delegate = nullptr;
	for (const auto DelegateLinkage : DelegatesLinkages) {
		if (DelegateLinkage.second->ViewSTTI() == ViewSTTI) {
			Delegate = DelegateLinkage.first;
		}
	}
	return Delegate;
}