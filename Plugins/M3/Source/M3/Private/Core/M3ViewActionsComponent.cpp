
#include "M3ViewActionsComponent.h"
#include "Engine/World.h"

UM3ViewActionsComponent::UM3ViewActionsComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3ViewActionsComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3ViewActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Actions.Num() != 0) {
		Actions[0]->OnUpdate(DeltaTime);
		if (Actions[0]->IsDone()) {
			Actions.RemoveAt(0);
		}
	}
}

UM3Action* UM3ViewActionsComponent::MoveToAction(float Duration, const FVector& Location) {
	const auto Action = NewObject<UM3MoveToAction>(this);
	Action->Setup(GetOwner(), Duration, Location);
	return Action;
}

UM3Action* UM3ViewActionsComponent::ScaleToAction(float Duration, const FVector& Scale) {
	const auto Action = NewObject<UM3ScaleToAction>(this);
	Action->Setup(GetOwner(), Duration, Scale);
	return Action;
}

UM3Action* UM3ViewActionsComponent::CallbackAction(float Duration, FM3ActionCallback Callback) {
	const auto Action = NewObject<UM3CallbackAction>(this);
	Action->Setup(GetOwner(), Duration, Callback);
	return Action;
}

void UM3ViewActionsComponent::RunAction(UM3Action* Action) {
	Actions.Add(Action);
}

UM3Action::UM3Action(){

}

void UM3Action::Setup(AActor* _Actor, float _Duration) {
	Actor = _Actor;
	Duration = _Duration;
}

void UM3Action::OnUpdate(float DeltaTime) {
	CurrentDuration += DeltaTime;
	if (CurrentDuration >= Duration) {
		bIsDone = true;
	}
}

bool UM3Action::IsDone() const {
	return bIsDone;
}

UM3Sequence::UM3Sequence() {

}

void UM3Sequence::Setup(AActor* _Actor) {
	
}

void UM3Sequence::OnUpdate(float DeltaTime) {
	if (Actions.Num() != 0) {
		Actions[0]->OnUpdate(DeltaTime);
		if (Actions[0]->IsDone()) {
			Actions.RemoveAt(0);
		}
	}
}

bool UM3Sequence::IsDone() const {
	return Actions.Num() == 0;
}

UM3MoveToAction::UM3MoveToAction() {

}

void UM3MoveToAction::Setup(AActor* _Actor, float _Duration, const FVector& Location) {
	UM3Action::Setup(_Actor, _Duration);
	MoveToLocation = Location;
	OldLocation = _Actor->GetActorLocation();
}

void UM3MoveToAction::OnUpdate(float DeltaTime) {
	UM3Action::OnUpdate(DeltaTime);
	Actor->SetActorLocation(FMath::Lerp(OldLocation, MoveToLocation, FMath::Clamp(CurrentDuration / Duration, 0.f, 1.f)));
}

UM3ScaleToAction::UM3ScaleToAction() {

}

void UM3ScaleToAction::Setup(AActor* _Actor, float _Duration, const FVector& Scale) {
	UM3Action::Setup(_Actor, _Duration);
	ScaleTo = Scale;
	OldScale = _Actor->GetActorScale();
}

void UM3ScaleToAction::OnUpdate(float DeltaTime) {
	UM3Action::OnUpdate(DeltaTime);
	Actor->SetActorScale3D(FMath::Lerp(OldScale, ScaleTo, FMath::Clamp(CurrentDuration / Duration, 0.f, 1.f)));
}

UM3CallbackAction::UM3CallbackAction() {

}

void UM3CallbackAction::Setup(AActor* _Actor, float _Duration, FM3ActionCallback _Callback) {
	UM3Action::Setup(_Actor, _Duration);
	Callback = _Callback;
}

void UM3CallbackAction::OnUpdate(float DeltaTime) {
	UM3Action::OnUpdate(DeltaTime);
	if (bIsDone) {
		Callback.ExecuteIfBound();
	}
}