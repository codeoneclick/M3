// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3EdModeProperties.generated.h"

FORWARD_DECL_STRONG(UM3BoardAssetsBundle)
FORWARD_DECL_STRONG(AM3BoardScheme)
FORWARD_DECL_STRONG(AM3CellScheme)
FORWARD_DECL_STRONG(AM3CellAppointmentScheme)

UCLASS()
class M3_API UM3EdModeProps_BoardCreate : public UObject
{
	GENERATED_BODY()

public:

	UM3EdModeProps_BoardCreate();

	UPROPERTY(Category = "Size", EditAnywhere, meta = (ShowForTools = "Tool_M3", ClampMin = 3, ClampMax = 12), BlueprintReadWrite)
	int Cols = 10;

	UPROPERTY(Category = "Size", EditAnywhere, meta = (ShowForTools = "Tool_M3", ClampMin = 3, ClampMax = 12), BlueprintReadWrite)
	int Rows = 10;

	UPROPERTY(Category = "Settings", meta = (ShowForTools = "Tool_M3", ClampMin = 10, ClampMax = 1000), EditAnywhere, BlueprintReadOnly)
	FVector2D ElementSize = FVector2D(100, 100);

	UPROPERTY(Category = "Bundle", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<UM3BoardAssetsBundle> AssetsBundle_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3BoardScheme> BoardScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellScheme> CellScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> FunctionalCellClosedScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> FunctionalCellHoleScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> FunctionalCellRandomScheme_BP;

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

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> SuperElementMatch4Scheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> SuperElementMatch5Scheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> SuperElementMatch6Scheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> SuperElementMatch7Scheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerBox1XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerBox2XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerBox3XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerIce1XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerIce2XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerWire1XScheme_BP;

	UPROPERTY(Category = "Scheme", EditAnywhere, meta = (ShowForTools = "Tool_M3_BP"), BlueprintReadWrite)
	TSubclassOf<AM3CellAppointmentScheme> BlockerWire2XScheme_BP;
};

UCLASS()
class M3_API UM3EdModeProps_BoardScheme : public UObject {
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool IsTurnBased;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int Duration;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int OneMatchScores;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	bool IsUseComboMatchScores;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	float ComboMatchScoresMultiplier;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int Star1Scores;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int Star2Scores;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	int Star3Scores;

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
class M3_API UM3EdModeProps_BoardReskin : public UObject
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

	UPROPERTY(Category = "Settings", meta = (ShowForTools = "Tool_M3", ClampMin = 10, ClampMax = 1000), EditAnywhere, BlueprintReadOnly)
	FVector2D ElementSize = FVector2D(100, 100);

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* CellMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* CellMesh;

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

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SuperElementMatch4Material;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SuperElementMatch4Mesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SuperElementMatch5Material;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SuperElementMatch5Mesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SuperElementMatch6Material;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SuperElementMatch6Mesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SuperElementMatch7Material;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SuperElementMatch7Mesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerBox1XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerBox1XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerBox2XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerBox2XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerBox3XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerBox3XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerIce1XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerIce1XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerIce2XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerIce2XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerWire1XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerWire1XMesh;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* BlockerWire2XMaterial;

	UPROPERTY(Category = "Assets", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* BlockerWire2XMesh;
};