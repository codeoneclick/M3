// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "M3GameGameMode.h"
#include "M3GamePlayerController.h"
#include "M3GamePawn.h"

AM3GameGameMode::AM3GameGameMode()
{
	// no pawn by default
	DefaultPawnClass = AM3GamePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AM3GamePlayerController::StaticClass();
}
