// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3App.generated.h"

FORWARD_DECL_STRONG(UM3CoordinatingComponent)
FORWARD_DECL_STRONG(UM3BoardGeneratorComponent)
FORWARD_DECL_STRONG(AM3BoardAssetsBundle)
FORWARD_DECL_STRONG(AM3BoardScheme)
FORWARD_DECL_STRONG(AM3CellAppointmentScheme)
FORWARD_DECL_STRONG(AM3ViewDelegates_API)

UCLASS()
class HEXMAP_API AM3App : public AActor
{
	GENERATED_BODY()
	
public:	

	AM3App();

protected:

	virtual void BeginPlay() override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	UM3CoordinatingComponent* CoordinatingComponent;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	UM3BoardGeneratorComponent* BoardGeneratorComponent;

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3TapGestureRecognizerComponent* TapGestureRecognizerComponent;

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3PanGestureRecognizerComponent* PanGestureRecognizerComponent;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3BoardAssetsBundle* AssetsBundle;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3ViewDelegates_API* Delegates_API;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3BoardScheme* Scheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellFunctionalSpawnScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementREDScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementGREENScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementBLUEScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementYELLOWScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementORANGEScheme;

	UPROPERTY(Category = "M3", EditAnywhere, BlueprintReadWrite)
	AM3CellAppointmentScheme* CellElementPURPLEScheme;
};
