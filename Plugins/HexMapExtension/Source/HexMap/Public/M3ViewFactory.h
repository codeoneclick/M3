// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "Components/ActorComponent.h"
#include "M3ViewFactory.generated.h"

FORWARD_DECL_STRONG(AM3Board)
FORWARD_DECL_STRONG(AM3Cell)
FORWARD_DECL_STRONG(AM3Element)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class HEXMAP_API UM3ViewFactory : public UObject {

	GENERATED_BODY()

public:

	UM3ViewFactory();
	~UM3ViewFactory();

	UPROPERTY(Category = "M3Views", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Board> Board_BP;

	UPROPERTY(Category = "M3Views", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Cell> Cell_BP;

	UPROPERTY(Category = "M3Views", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Element> Element_BP;

	AM3Board* CreateBoard(UWorld* World);
	AM3Cell* CreateCell(UWorld* World);
	AM3Element* CreateElement(UWorld* World);
};
