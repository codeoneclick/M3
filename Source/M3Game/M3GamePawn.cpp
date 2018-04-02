// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "M3GamePawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AM3GamePawn::AM3GamePawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AM3GamePawn::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AM3GamePawn::SetupPlayerInputComponent(UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
}

void AM3GamePawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) {
	Super::CalcCamera(DeltaTime, OutResult);
	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}