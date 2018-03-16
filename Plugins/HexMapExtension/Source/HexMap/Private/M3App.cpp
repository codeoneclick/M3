// Fill out your copyright notice in the Description page of Project Settings.

#include "M3App.h"
#include "M3Board.h"
#include "M3CoordinatingComponent.h"
#include "M3AssetsBundle.h"
#include "M3ViewFactory.h"
#include "M3Scheme.h"
#include "Engine/World.h"
#include "M3TapGestureRecognizerComponent.h"
#include "M3PanGestureRecognizerComponent.h"

AM3App::AM3App() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("M3AppRootComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);

	CoordinatingComponent = CreateDefaultSubobject<UM3CoordinatingComponent>(TEXT("CoordinatingComponent"));

	TapGestureRecognizerComponent = CreateDefaultSubobject<UM3TapGestureRecognizerComponent>(TEXT("TapGestureRecognizerComponent"));
	PanGestureRecognizerComponent = CreateDefaultSubobject<UM3PanGestureRecognizerComponent>(TEXT("PanGestureRecognizerComponent"));
}

void AM3App::BeginPlay() {
	Super::BeginPlay();

	CoordinatingComponent = static_cast<UM3CoordinatingComponent*>(GetComponentByClass(UM3CoordinatingComponent::StaticClass()));
	assert(CoordinatingComponent != nullptr);

	TapGestureRecognizerComponent = FindComponentByClass<UM3TapGestureRecognizerComponent>();
	if (!TapGestureRecognizerComponent) {
		UE_LOG(LogTemp, Error, TEXT("Can't find TapGestureRecognizerComponent"));
	}

	PanGestureRecognizerComponent = FindComponentByClass<UM3PanGestureRecognizerComponent>();
	if (!PanGestureRecognizerComponent) {
		UE_LOG(LogTemp, Error, TEXT("Can't find PanGestureRecognizerComponent"));
	}

	const auto PC = GetWorld()->GetFirstPlayerController();
	const auto CustomInputComponent = PC->InputComponent;

	CustomInputComponent->BindTouch(IE_Pressed, this, &AM3App::OnTouchPressed);
	CustomInputComponent->BindTouch(IE_Released, this, &AM3App::OnTouchReleased);

	AssetsBundle = static_cast<UM3BoardAssetsBundle*>(NewObject<UM3AssetsBundle>(this, AssetsBundle_BP));

	CoordinatingComponent->CreateModels();
	CoordinatingComponent->CreateControllers();
	CoordinatingComponent->CreateViews(AssetsBundle);
	CoordinatingComponent->OnModelChanged(BoardScheme);

	CoordinatingComponent->OnStart();
}

void AM3App::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3App::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location) {
	TapGestureRecognizerComponent->OnTouchPressed(FingerIndex, Location);
	PanGestureRecognizerComponent->OnTouchPressed(FingerIndex, Location);
}

void AM3App::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location) {
	TapGestureRecognizerComponent->OnTouchReleased(FingerIndex, Location);
	PanGestureRecognizerComponent->OnTouchReleased(FingerIndex, Location);
}


