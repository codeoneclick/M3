// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3Scheme.generated.h"

UENUM(BlueprintType)
enum class EM3CellAppointment : uint8
{
	UNKNOWN UMETA(DisplayName = "Unknown"),
	FUNCTIONAL UMETA(DisplayName = "Functional"),
	ELEMENT UMETA(DispalyName = "Element"),
	BLOCKER UMETA(DispalyName = "Blocker"),
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
	int Id;

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
	TArray<AM3CellAppointmentScheme*> Appointments;
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
};
