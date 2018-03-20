// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3Scheme.generated.h"

UENUM(BlueprintType)
enum class EM3CellAppointment : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	FUNCTIONAL UMETA(DisplayName = "Functional"),
	ELEMENT UMETA(DispalyName = "Element"),
	BLOCKER UMETA(DispalyName = "Blocker"),
	SUPERELEMENT UMETA(DispalyName = "Superelement"),
};

UENUM(BlueprintType)
enum class EM3ElementId : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	ELEMENT_SPAWNER UMETA(DisplayName = "Element Spawner"),
	ELEMENT_RED UMETA(DisplayName = "Element Red"),
	ELEMENT_GREEN UMETA(DisplayName = "Element Green"),
	ELEMENT_BLUE UMETA(DisplayName = "Element Blue"),
	ELEMENT_YELLOW UMETA(DisplayName = "Element Yellow"),
	ELEMENT_ORANGE UMETA(DisplayName = "Element Orange"),
	ELEMENT_PURPLE UMETA(DisplayName = "Element Purple"),
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class HEXMAP_API AM3Scheme_INTERFACE : public AActor {
	GENERATED_BODY()

public:

};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class HEXMAP_API AM3CellAppointmentScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	EM3CellAppointment Appointment;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	EM3ElementId Id;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	int GroupId;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class HEXMAP_API AM3CellScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

public:

	AM3CellScheme();

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Col;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Row;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3CellAppointmentScheme*> Appointments;

	void AddAppointment(AM3CellAppointmentScheme* Appointment);
	AM3CellAppointmentScheme* GetAppointment(EM3CellAppointment AppointmentId) const;

#if WITH_EDITOR

	static AM3CellAppointmentScheme* EdModeSelectedAppointmentScheme;

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void OnEditorTick(float DeltaTime);

#endif
};


UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class HEXMAP_API AM3GoalScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	EM3ElementId Id;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	int Quantity;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	bool Enabled;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Scheme))
class HEXMAP_API AM3BoardScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Cols;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Rows;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	FVector2D ElementSize;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementRedScheme;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementGreenScheme;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementBlueScheme;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementYellowScheme;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementOrangeScheme;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	AM3CellAppointmentScheme* ElementPurpleScheme;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3CellScheme*> Cells;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	int Turns;

	UPROPERTY(Category = "M3Scheme", VisibleAnywhere, BlueprintReadOnly)
	TArray<AM3GoalScheme*> Goals;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadOnly)
	TSet<EM3ElementId> ElementIds;

	AM3GoalScheme* GetGoalScheme(UWorld* World, EM3ElementId Id);
};
