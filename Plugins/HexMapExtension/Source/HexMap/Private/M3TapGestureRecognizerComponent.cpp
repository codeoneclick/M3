#include "M3TapGestureRecognizerComponent.h"
#include "M3TapGestureResponderComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"

UM3TapGestureRecognizerComponent::UM3TapGestureRecognizerComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3TapGestureRecognizerComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3TapGestureRecognizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UM3TapGestureRecognizerComponent::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location) {
	bIsInterrupted = FingerIndex != ETouchIndex::Touch1;
}

void UM3TapGestureRecognizerComponent::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location) {
	if (!bIsInterrupted) {
		for (TObjectIterator<AActor> ActorIt; ActorIt; ++ActorIt) {
			AActor* Actor = *ActorIt;
			UM3TapGestureResponderComponent* TapGestureResponderComponent = Actor->FindComponentByClass<UM3TapGestureResponderComponent>();
			if (TapGestureResponderComponent) {
				TapGestureResponderComponent->OnTapDelegate.ExecuteIfBound(FingerIndex, Location);
			}
		}
	}
}


