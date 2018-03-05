// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3App.generated.h"

FORWARD_DECL_STRONG(UM3CoordinatingComponent)
FORWARD_DECL_STRONG(UM3BoardGeneratorComponent)
FORWARD_DECL_STRONG(AM3BoardAssetsBundle)
FORWARD_DECL_STRONG(UM3BoardScheme)
FORWARD_DECL_STRONG(UM3CellScheme)
FORWARD_DECL_STRONG(UM3CellAppointmentScheme)
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

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	AM3BoardAssetsBundle* AssetsBundle;

	UPROPERTY(Category = "M3Delegates", EditAnywhere, BlueprintReadWrite)
	AM3ViewDelegates_API* Delegates_API;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3BoardScheme> BoardScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellScheme> CellScheme_BP; 

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementSpawnerScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementRedScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementGreenScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementBlueScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementYellowScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementOrangeScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3CellAppointmentScheme> ElementPurpleScheme_BP;
};
