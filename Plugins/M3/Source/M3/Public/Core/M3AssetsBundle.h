// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Asset.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3AssetsBundle.generated.h"

FORWARD_DECL_STRONG(AM3Board)
FORWARD_DECL_STRONG(AM3Cell)
FORWARD_DECL_STRONG(AM3Regularelement)
FORWARD_DECL_STRONG(AM3Superelement)

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

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Cell;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset CellHole;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset CellClosed;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_RED;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_GREEN;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_BLUE;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_YELLOW;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_ORANGE;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_PURPLE;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Board> Board_BP;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Cell> Cell_BP;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Regularelement> Regularelement_BP;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3Superelement> Superelement_BP;

	UFUNCTION(BlueprintCallable, Category = "M3Bundle")
	AM3Board* ConstructBoard(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "M3Bundle")
	AM3Cell* ConstructCell(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "M3Bundle")
	AM3Regularelement* ConstructRegularelement(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "M3Bundle")
	AM3Superelement* ConstructSuperelement(UWorld* World);
};

