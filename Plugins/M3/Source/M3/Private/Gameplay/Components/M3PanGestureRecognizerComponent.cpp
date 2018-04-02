// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3PanGestureRecognizerComponent.h"
#include "M3PanGestureResponderComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"
#include "Engine/World.h"

UM3PanGestureRecognizerComponent::UM3PanGestureRecognizerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3PanGestureRecognizerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UM3PanGestureRecognizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bIsInterrupted) {
		float Finger1LocationX = 0.f;
		float Finger1LocationY = 0.f;
		float Finger2LocationX = 0.f;
		float Finger2LocationY = 0.f;
		bool bIsFinger1Touched = UM3PanGestureRecognizerComponent::IsFinger1Touched(GetWorld(), Finger1LocationX, Finger1LocationY);
		bool bIsFinger2Touched = UM3PanGestureRecognizerComponent::IsFinger2Touched(GetWorld(), Finger2LocationX, Finger2LocationY);
		if (bIsFinger1Touched && !bIsFinger2Touched) {
			float PanDeltaX = Finger1LocationX - PrevTouchLocationX;
			float PanDeltaY = Finger1LocationY - PrevTouchLocationY;
			if (!FMath::IsNearlyZero(PanDeltaX) || !FMath::IsNearlyZero(PanDeltaY)) {
				for (TObjectIterator<AActor> ActorIt; ActorIt; ++ActorIt) {
					AActor* Actor = *ActorIt;
					UM3PanGestureResponderComponent* PanGestureResponderComponent = Actor->FindComponentByClass<UM3PanGestureResponderComponent>();
					if (PanGestureResponderComponent) {
						if (bIsPanned) {
							PanGestureResponderComponent->OnPanDelegate.ExecuteIfBound(EM3PanState::MOVE, FVector(Finger1LocationX, Finger1LocationY, 0.f), FVector(PanDeltaX, PanDeltaY, 0.f));
						} else {
							PanGestureResponderComponent->OnPanDelegate.ExecuteIfBound(EM3PanState::START, FVector(Finger1LocationX, Finger1LocationY, 0.f), FVector(0.f, 0.f, 0.f));
						}
					}
				}
			}
			PrevTouchLocationX = Finger1LocationX;
			PrevTouchLocationY = Finger1LocationY;
			bIsPanned = true;
		}
	}
}

void UM3PanGestureRecognizerComponent::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location) {
	bool bIsFinger1Touched = UM3PanGestureRecognizerComponent::IsFinger1Touched(GetWorld());
	bool bIsFinger2Touched = UM3PanGestureRecognizerComponent::IsFinger2Touched(GetWorld());
	bIsInterrupted = !bIsFinger1Touched || bIsFinger2Touched;
	if (!bIsInterrupted) {
		PrevTouchLocationX = Location.X;
		PrevTouchLocationY = Location.Y;
	}
	bIsPanned = false;
}

void UM3PanGestureRecognizerComponent::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location) {
	PrevTouchLocationX = Location.X;
	PrevTouchLocationY = Location.Y;
	if (!bIsInterrupted) {
		for (TObjectIterator<AActor> ActorIt; ActorIt; ++ActorIt) {
			AActor* Actor = *ActorIt;
			UM3PanGestureResponderComponent* PanGestureResponderComponent = Actor->FindComponentByClass<UM3PanGestureResponderComponent>();
			if (PanGestureResponderComponent) {
				PanGestureResponderComponent->OnPanDelegate.ExecuteIfBound(EM3PanState::END, FVector(PrevTouchLocationX, PrevTouchLocationY, 0.f), FVector(0.f, 0.f, 0.f));
			}
		}
	}
	bIsPanned = false;
}

bool UM3PanGestureRecognizerComponent::IsFinger1Touched(UWorld* World)
{
	float LocationX = 0.f;
	float LocationY = 0.f;
	return IsFinger1Touched(World, LocationX, LocationY);
}

bool UM3PanGestureRecognizerComponent::IsFinger2Touched(UWorld* World)
{
	float LocationX = 0.f;
	float LocationY = 0.f;
	return IsFinger2Touched(World, LocationX, LocationY);
}

bool UM3PanGestureRecognizerComponent::IsFinger1Touched(UWorld* World, float& LocationX, float& LocationY)
{
	bool bResult;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->GetInputTouchState(ETouchIndex::Touch1, LocationX, LocationY, bResult);
	return bResult;
}

bool UM3PanGestureRecognizerComponent::IsFinger2Touched(UWorld* World, float& LocationX, float& LocationY)
{
	bool bResult;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->GetInputTouchState(ETouchIndex::Touch2, LocationX, LocationY, bResult);
	return bResult;
}


