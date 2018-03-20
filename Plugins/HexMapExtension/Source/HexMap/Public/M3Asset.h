// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "M3Asset.generated.h"

UENUM(BlueprintType)
enum class EM3Asset : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	BIN UMETA(DisplayName = "Bin"),
	MATERIAL UMETA(DispalyName = "Material"),
	MESH UMETA(DispalyName = "Mesh"),
	COMPOSITE UMETA(DispalyName = "Composite"),
};

USTRUCT(Blueprintable, BlueprintType)
struct HEXMAP_API FM3Asset
{
	GENERATED_USTRUCT_BODY()

public:

	FM3Asset();
	~FM3Asset();

	UPROPERTY(Category = "M3Asset", VisibleAnywhere)
	EM3Asset Asset;

	UPROPERTY(Category = "M3Asset", VisibleAnywhere)
	FString Filepath;
};

USTRUCT(Blueprintable, BlueprintType)
struct HEXMAP_API FM3BoardAsset : public FM3Asset
{
	GENERATED_USTRUCT_BODY()

public:

	FM3BoardAsset();
	~FM3BoardAsset();

	UPROPERTY(Category = "M3Asset", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Material;

	UPROPERTY(Category = "M3Asset", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* Mesh;

	UPROPERTY(Category = "M3Asset", EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* AnimatedMesh;

};