// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3AssetsBundle.h"
#include "M3Board.h"
#include "M3Cell.h"
#include "M3Element.h"
#include "M3Blocker.h"
#include "M3RegularElement.h"
#include "M3SuperElement.h"
#include "Engine/World.h"

AM3Board* UM3BoardAssetsBundle::ConstructBoard(UWorld* World) {
	AM3Board* Result = nullptr;
	if (Board_BP) {
		Result = World->SpawnActor<AM3Board>(Board_BP);
	} else {
		Result = World->SpawnActor<AM3Board>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}

AM3Cell* UM3BoardAssetsBundle::ConstructCell(UWorld* World) {
	AM3Cell* Result = nullptr;
	if (Cell_BP) {
		Result = World->SpawnActor<AM3Cell>(Cell_BP);
	} else {
		Result = World->SpawnActor<AM3Cell>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}

AM3Element* UM3BoardAssetsBundle::ConstructElement(UWorld* World) {
	AM3Element* Result = nullptr;
	Result = World->SpawnActor<AM3Element>(FVector(0.f), FRotator(0.f));
	return Result;
}

AM3RegularElement* UM3BoardAssetsBundle::ConstructRegularElement(UWorld* World) {
	AM3RegularElement* Result = nullptr;
	if (RegularElement_BP) {
		Result = World->SpawnActor<AM3RegularElement>(RegularElement_BP);
	} else {
		Result = World->SpawnActor<AM3RegularElement>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}

AM3SuperElement* UM3BoardAssetsBundle::ConstructSuperElement(UWorld* World) {
	AM3SuperElement* Result = nullptr;
	if (SuperElement_BP) {
		Result = World->SpawnActor<AM3SuperElement>(SuperElement_BP);
	}
	else {
		Result = World->SpawnActor<AM3SuperElement>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}

AM3Blocker* UM3BoardAssetsBundle::ConstructBlocker(UWorld* World) {
	AM3Blocker* Result = nullptr;
	if (SuperElement_BP) {
		Result = World->SpawnActor<AM3Blocker>(Blocker_BP);
	}
	else {
		Result = World->SpawnActor<AM3Blocker>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}