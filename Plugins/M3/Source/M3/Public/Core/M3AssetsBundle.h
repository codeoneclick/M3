// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Asset.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3AssetsBundle.generated.h"

FORWARD_DECL_STRONG(AM3Board)
FORWARD_DECL_STRONG(AM3Cell)
FORWARD_DECL_STRONG(AM3Element)
FORWARD_DECL_STRONG(AM3RegularElement)
FORWARD_DECL_STRONG(AM3SuperElement)
FORWARD_DECL_STRONG(AM3Blocker)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Bundle))
class M3_API UM3AssetsBundle : public UObject
{
	GENERATED_BODY()

public:

};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Bundle))
class M3_API UM3BoardAssetsBundle : public UM3AssetsBundle
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Cell;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset CellHole;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset CellClosed;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_RED;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_GREEN;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_BLUE;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_YELLOW;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_ORANGE;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_PURPLE;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset SuperElement_MATCH4;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset SuperElement_MATCH5;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset SuperElement_MATCH6;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset SuperElement_MATCH7;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Box1X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Box2X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Box3X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Ice1X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Ice2X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Wire1X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Blocker_Wire2X;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Board> Board_BP = nullptr;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Cell> Cell_BP = nullptr;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Element> Element_BP = nullptr;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3RegularElement> RegularElement_BP = nullptr;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3SuperElement> SuperElement_BP = nullptr;

	UPROPERTY(Category = "Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Blocker> Blocker_BP = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3Board* ConstructBoard(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3Cell* ConstructCell(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3Element* ConstructElement(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3RegularElement* ConstructRegularElement(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3SuperElement* ConstructSuperElement(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Bundle")
	AM3Blocker* ConstructBlocker(UWorld* World);
};

