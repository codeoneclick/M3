// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3Scheme.generated.h"

UENUM(BlueprintType)
enum class EM3CellAppointment : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	FUNCTIONAL UMETA(DisplayName = "Functional"),
	REGULARELEMENT UMETA(DispalyName = "Regular Element"),
	SUPERELEMENT UMETA(DispalyName = "Super Element"),
	BLOCKER UMETA(DispalyName = "Blocker"),
};

UENUM(BlueprintType)
enum class EM3ElementId : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	CELL_CLOSED UMETA(DisplayName = "Cell Closed"),
	CELL_HOLE UMETA(DisplayName = "Cell Hole"),
	CELL_RANDOM UMETA(DisplayName = "Cell Random"),
	ELEMENT_SPAWNER UMETA(DisplayName = "Element Spawner"),
	ELEMENT_RED UMETA(DisplayName = "Element Red"),
	ELEMENT_GREEN UMETA(DisplayName = "Element Green"),
	ELEMENT_BLUE UMETA(DisplayName = "Element Blue"),
	ELEMENT_YELLOW UMETA(DisplayName = "Element Yellow"),
	ELEMENT_ORANGE UMETA(DisplayName = "Element Orange"),
	ELEMENT_PURPLE UMETA(DisplayName = "Element Purple"),
	SUPERELEMENT_MATCH4 UMETA(DisplayName = "Super element Match 4"),
	SUPERELEMENT_MATCH5 UMETA(DisplayName = "Super element Match 5"),
	SUPERELEMENT_MATCH6 UMETA(DisplayName = "Super element Match 6"),
	SUPERELEMENT_MATCH7 UMETA(DisplayName = "Super element Match 7"),
	BLOCKER_NONE UMETA(DisplayName = "Blocker None"),
	BLOCKER_BOX1X UMETA(DisplayName = "Box 1X"),
	BLOCKER_BOX2X UMETA(DisplayName = "Box 2X"),
	BLOCKER_BOX3X UMETA(DisplayName = "Box 3X"),
	BLOCKER_ICE1X UMETA(DisplayName = "Ice 1X"),
	BLOCKER_ICE2X UMETA(DisplayName = "Ice 2X"),
	BLOCKER_WIRE1X UMETA(DisplayName = "Wire 1X"),
	BLOCKER_WIRE2X UMETA(DisplayName = "Wire 2X"),
};

UENUM(BlueprintType)
enum class EM3GoalId : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	COLLECT_RED_ELEMENTS UMETA(DisplayName = "Collect Red element"),
	COLLECT_GREEN_ELEMENTS UMETA(DisplayName = "Collect Green element"),
	COLLECT_BLUE_ELEMENTS UMETA(DisplayName = "Collect Blue element"),
	COLLECT_YELLOW_ELEMENTS UMETA(DisplayName = "Collect Yellow element"),
	COLLECT_ORANGE_ELEMENTS UMETA(DisplayName = "Collect Orange element"),
	COLLECT_PURPLE_ELEMENTS UMETA(DisplayName = "Collect Purple element"),
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class M3_API AM3Scheme_INTERFACE : public AActor {
	GENERATED_BODY()

public:

};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class M3_API AM3CellAppointmentScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	EM3CellAppointment Appointment;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	EM3ElementId Id;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int GroupId;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class M3_API AM3CellScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

public:

	AM3CellScheme();

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Col;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Row;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3CellAppointmentScheme*> Appointments;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* CellClosedMaterial;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMesh* CellClosedMesh;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* CellHoleMaterial;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMesh* CellHoleMesh;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* CellRandomMaterial;

	UPROPERTY(Category = "Asset", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMesh* CellRandomMesh;

	UPROPERTY(Category = "Component", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ElementMeshComponent;

	UPROPERTY(Category = "Component", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* BlockerMeshComponent;

	void AddAppointment(AM3CellAppointmentScheme* Appointment);
	void RemoveAppointment(EM3CellAppointment AppointmentId);
	void RemoveAllAppointments();
	AM3CellAppointmentScheme* GetAppointment(EM3CellAppointment AppointmentId) const;
	bool IsAppointmentExist(EM3CellAppointment AppointmentId) const;

	static AM3CellAppointmentScheme* EdModeSelectedAppointmentScheme;

#if WITH_EDITOR

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void OnEditorTick(float DeltaTime);

#endif
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class M3_API AM3GoalScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	AM3GoalScheme();

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	EM3GoalId Id;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int Quantity;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	bool Enabled;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class M3_API AM3BoardScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	AM3BoardScheme();

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Cols;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Rows;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	FVector2D ElementSize;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* FunctionalCellClosedScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* FunctionalCellHoleScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* FunctionalCellRandomScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementRedScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementGreenScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementBlueScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementYellowScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementOrangeScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementPurpleScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* SuperElementMatch4Scheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* SuperElementMatch5Scheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* SuperElementMatch6Scheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* SuperElementMatch7Scheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerBox1XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerBox2XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerBox3XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerIce1XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerIce2XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerWire1XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerWire2XScheme;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* BlockerNoneScheme;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3CellScheme*> Cells;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	bool IsTurnBased;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Duration;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int OneMatchScores;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	bool IsUseComboMatchScores;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	float ComboMatchScoresMultiplier;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Star1Scores;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Star2Scores;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Star3Scores;

	UPROPERTY(Category = "Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3GoalScheme*> Goals;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadOnly)
	TSet<EM3ElementId> ElementIds;

	AM3GoalScheme* GetGoalScheme(UWorld* World, EM3GoalId Id);
};
