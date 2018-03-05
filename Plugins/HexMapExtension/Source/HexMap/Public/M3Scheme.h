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

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Scheme))
class HEXMAP_API UM3Scheme_INTERFACE : public UObject {
	GENERATED_BODY()

public:

};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Scheme))
class HEXMAP_API UM3CellAppointmentScheme : public UM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	EM3CellAppointment Appointment;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	EM3ElementId Id;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int GroupId;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Scheme))
class HEXMAP_API UM3CellScheme : public UM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int Col;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int Row;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	TSet<UM3CellAppointmentScheme*> Appointments;

	void AddAppointment(UM3CellAppointmentScheme* Appointment);
	UM3CellAppointmentScheme* GetAppointment(EM3CellAppointment AppointmentId) const;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Scheme))
class HEXMAP_API UM3BoardScheme : public UM3Scheme_INTERFACE {

	GENERATED_BODY()

public:

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int Cols;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	int Rows;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	TArray<UM3CellScheme*> Cells;

	UPROPERTY(Category = "Scheme", EditAnywhere, BlueprintReadWrite)
	TSet<EM3ElementId> ElementIds;
};
