// Fill out your copyright notice in the Description page of Project Settings.

#include "M3AssetsBundle.h"
#include "M3Board.h"
#include "M3Cell.h"
#include "M3Element.h"
#include "Engine/World.h"

AM3Board* UM3BoardAssetsBundle::ConstructBoard(UWorld* World) {
	AM3Board* Result = nullptr;
	if (Board_BP) {
		Result = World->SpawnActor<AM3Board>(Board_BP);
	}
	return Result;
}

AM3Cell* UM3BoardAssetsBundle::ConstructCell(UWorld* World) {
	AM3Cell* Result = nullptr;
	if (Cell_BP) {
		Result = World->SpawnActor<AM3Cell>(Cell_BP);
	}
	return Result;
}

AM3Element* UM3BoardAssetsBundle::ConstructElement(UWorld* World) {
	AM3Element* Result = nullptr;
	if (Element_BP) {
		Result = World->SpawnActor<AM3Element>(Element_BP);
	}
	return Result;
}