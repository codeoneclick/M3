// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "HMEdModeProperties.generated.h"

FORWARD_DECL_STRONG(UM3BoardAssetsBundle)
FORWARD_DECL_STRONG(AM3BoardScheme)
FORWARD_DECL_STRONG(AM3CellScheme)
FORWARD_DECL_STRONG(AM3CellAppointmentScheme)

UCLASS()
class HEXMAP_API UHMEdModePropertiesAddCircle : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHMTile> Tile_BP;

	UPROPERTY(Category = "Radius", EditAnywhere, meta = (ShowForTools = "Tool_Radius", ClampMin = 0, ClampMax = 8), BlueprintReadWrite)
	int32 Radius = 2;

	UPROPERTY(Category = "Location", EditAnywhere, meta = (ShowForTools = "Tool_Location"), BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

UCLASS()
class HEXMAP_API UHMEdModePropertiesAddRectangle : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHMTile> Tile_BP;

	UPROPERTY(Category = "SizeX", EditAnywhere, meta = (ShowForTools = "Tool_Size", ClampMin = 1, ClampMax = 16), BlueprintReadWrite)
	int32 SizeX = 2;

	UPROPERTY(Category = "SizeY", EditAnywhere, meta = (ShowForTools = "Tool_Size", ClampMin = 1, ClampMax = 16), BlueprintReadWrite)
	int32 SizeY = 2;

	UPROPERTY(Category = "Location", EditAnywhere, meta = (ShowForTools = "Tool_Location"), BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

UCLASS()
class HEXMAP_API UHMEdModePropertiesAddTile : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHMTile> Tile_BP;
};

UCLASS()
class HEXMAP_API UHMEdModePropertiesRandomizeTiles : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Randomizer Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Randomizer_BP"), BlueprintReadWrite)
	TSubclassOf<class AHMTileRandomizer> Randomizer_BP;
};

UCLASS()
class HEXMAP_API UHMEdModePropertiesTileBatchApplier : public UObject
{
	GENERATED_BODY()

public:

};

UCLASS()
class HEXMAP_API UHMEdModePropertiesSetTileSize : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Size", EditAnywhere, meta = (ShowForTools = "Tool_Tile_Size", ClampMin = 1.f, ClampMax = 1000.f), BlueprintReadWrite)
	float TileSize = 200.f;
};

UCLASS()
class HEXMAP_API UM3EdModeProps_BoardCreate : public UObject
{
	GENERATED_BODY()

public:

	UM3EdModeProps_BoardCreate();

	UPROPERTY(Category = "Size", EditAnywhere, meta = (ShowForTools = "Tool_M3", ClampMin = 3, ClampMax = 12), BlueprintReadWrite)
	int Cols = 10;

	UPROPERTY(Category = "Size", EditAnywhere, meta = (ShowForTools = "Tool_M3", ClampMin = 3, ClampMax = 12), BlueprintReadWrite)
	int Rows = 10;

	UPROPERTY(Category = "Bundle", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<UM3BoardAssetsBundle> AssetsBundle_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3BoardScheme> BoardScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellScheme> CellScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> FunctionalSpawnScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementRedScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementGreenScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementBlueScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementYellowScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementOrangeScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> ElementPurpleScheme_BP;
};

UCLASS()
class HEXMAP_API UM3EdModeProps_BoardScheme : public UObject {
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int Turns;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool RedElementExist;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool GreenElementExist;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool BlueElementExist;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool YellowElementExist;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool OrangeElementExist;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool PurpleElementExist;

	UPROPERTY(Category = "Settings", meta = (ShowForTools = "Tool_M3", ClampMin = 10, ClampMax = 1000), EditAnywhere, BlueprintReadOnly)
	FVector2D ElementSize = FVector2D(100, 100);

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectRedElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int RedElementsQuantity;

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectGreenElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int GreenElementsQuantity;

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectBlueElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int BlueElementsQuantity;

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectYellowElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int YellowElementsQuantity;

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectOrangeElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int OrangeElementsQuantity;

	UPROPERTY(Category = "Goals", EditAnywhere, BlueprintReadWrite)
	bool CollectPurpleElements;

	UPROPERTY(Category = "Goals", meta = (ShowForTools = "Tool_M3", ClampMin = 1, ClampMax = 1000), EditAnywhere, BlueprintReadWrite)
	int PurpleElementsQuantity;
};

UCLASS()
class HEXMAP_API UM3EdModeProps_BoardReskin : public UObject
{
	GENERATED_BODY()

protected:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

#endif

public:

	UPROPERTY(Category = "Bundle", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<UM3BoardAssetsBundle> AssetsBundle_BP;

	UPROPERTY(Category = "Bundle", VisibleAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadOnly)
	UM3BoardAssetsBundle* AssetsBundle;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RedElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* RedElementMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* GreenElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* GreenElementMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlueElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlueElementMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* YellowElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* YellowElementMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* OrangeElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* OrangeElementMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* PurpleElementMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* PurpleElementMesh;
};