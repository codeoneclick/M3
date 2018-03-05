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
};

UENUM(BlueprintType)
enum class EM3ElementId : uint8 {
	UNKNOWN UMETA(DisplayName = "Unknown"),
	SPAWNER UMETA(DisplayName = "Spawner"),
	ELEMENT_RED UMETA(DisplayName = "Element RED"),
	ELEMENT_GREEN UMETA(DisplayName = "Element GREEN"),
	ELEMENT_BLUE UMETA(DisplayName = "Element BLUE"),
	ELEMENT_YELLOW UMETA(DisplayName = "Element YELLOW"),
	ELEMENT_ORANGE UMETA(DisplayName = "Element ORANGE"),
	ELEMENT_PURPLE UMETA(DisplayName = "Element PURPLE"),
};

UCLASS()
class HEXMAP_API AM3Scheme_INTERFACE : public AActor
{
	GENERATED_BODY()

public:
	AM3Scheme_INTERFACE();
	~AM3Scheme_INTERFACE();
};

UCLASS()
class HEXMAP_API AM3CellAppointmentScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	AM3CellAppointmentScheme();
	~AM3CellAppointmentScheme();

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	EM3CellAppointment Appointment;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	EM3ElementId Id;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	int GroupId;
};

UCLASS()
class HEXMAP_API AM3CellScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:
	AM3CellScheme();
	~AM3CellScheme();

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	int Col;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	int Row;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	TSet<AM3CellAppointmentScheme*> Appointments;

	void AddAppointment(AM3CellAppointmentScheme* Appointment);
	AM3CellAppointmentScheme* GetAppointment(EM3CellAppointment AppointmentId) const;
};

UCLASS()
class HEXMAP_API AM3BoardScheme : public AM3Scheme_INTERFACE {

	GENERATED_BODY()

public:
	AM3BoardScheme();
	~AM3BoardScheme();

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	int Cols;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	int Rows;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	TArray<AM3CellScheme*> Cells;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	TSet<EM3ElementId> ElementIds;
};
