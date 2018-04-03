// Copyright serhii serhiiv 2018 All rights reserved.

#include "M32TapGestureRecognizerComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"
#include "M32TapGestureResponderComponent.h"

UM32TapGestureRecognizerComponent::UM32TapGestureRecognizerComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM32TapGestureRecognizerComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM32TapGestureRecognizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentTime += DeltaTime;
}

void UM32TapGestureRecognizerComponent::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location) {
	if (CurrentTime - LastTapTime < TimeInterval) {
		LastTapTime = 0.f;
		for (TObjectIterator<AActor> ActorIt; ActorIt; ++ActorIt) {
			AActor* Actor = *ActorIt;
			UM32TapGestureResponderComponent* DoubleTapGestureResponderComponent = Actor->FindComponentByClass<UM32TapGestureResponderComponent>();
			if (DoubleTapGestureResponderComponent) {
				DoubleTapGestureResponderComponent->On2TapDelegate.ExecuteIfBound(FingerIndex, Location);
			}
		}
	} else {
		LastTapTime = CurrentTime;
	}
}

void UM32TapGestureRecognizerComponent::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location) {

}

