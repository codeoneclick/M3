// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3AssetsBundle.h"
#include "M3Board.h"
#include "M3Cell.h"
#include "M3Element.h"
#include "M3Regularelement.h"
#include "M3Superelement.h"
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

AM3Regularelement* UM3BoardAssetsBundle::ConstructRegularelement(UWorld* World) {
	AM3Regularelement* Result = nullptr;
	if (Regularelement_BP) {
		Result = World->SpawnActor<AM3Regularelement>(Regularelement_BP);
	} else {
		Result = World->SpawnActor<AM3Regularelement>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}

AM3Superelement* UM3BoardAssetsBundle::ConstructSuperelement(UWorld* World) {
	AM3Superelement* Result = nullptr;
	if (Superelement_BP) {
		Result = World->SpawnActor<AM3Superelement>(Superelement_BP);
	}
	else {
		Result = World->SpawnActor<AM3Superelement>(FVector(0.f), FRotator(0.f));
	}
	return Result;
}