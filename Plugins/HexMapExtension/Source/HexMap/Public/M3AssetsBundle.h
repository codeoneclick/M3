// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Asset.h"
#include "M3AssetsBundle.generated.h"

UCLASS()
class HEXMAP_API AM3AssetsBundle : public AActor
{
	GENERATED_BODY()

public:

	AM3AssetsBundle();
	~AM3AssetsBundle();
};

UCLASS()
class HEXMAP_API AM3BoardAssetsBundle : public AM3AssetsBundle
{
	GENERATED_BODY()

public:

	AM3BoardAssetsBundle();
	~AM3BoardAssetsBundle();

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Cell;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_RED;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_GREEN;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_BLUE;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_YELLOW;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_ORANGE;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	FM3BoardAsset Element_PURPLE;
};

