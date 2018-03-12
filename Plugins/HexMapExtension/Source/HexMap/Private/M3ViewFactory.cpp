// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ViewFactory.h"
#include "M3Board.h"
#include "M3Cell.h"
#include "M3Element.h"
#include "Engine/World.h"

UM3ViewFactory::UM3ViewFactory() {
}

UM3ViewFactory::~UM3ViewFactory() {
}

AM3Board* UM3ViewFactory::CreateBoard(UWorld* World) {
	AM3Board* Board = nullptr;
	if (Board_BP) {
		Board = World->SpawnActor<AM3Board>(Board_BP);
	}
	return Board;
}

AM3Cell* UM3ViewFactory::CreateCell(UWorld* World) {
	AM3Cell* Cell = nullptr;
	if (Cell_BP) {
		Cell = World->SpawnActor<AM3Cell>(Cell_BP);
	}
	return Cell;
}

AM3Element* UM3ViewFactory::CreateElement(UWorld* World) {
	AM3Element* Element = nullptr;
	if (Element_BP) {
		Element = World->SpawnActor<AM3Element>(Element_BP);
	}
	return Element;
}