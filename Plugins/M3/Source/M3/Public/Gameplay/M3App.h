// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3App.generated.h"

FORWARD_DECL_STRONG(UM3CoordinatingComponent)
FORWARD_DECL_STRONG(UM3BoardAssetsBundle)
FORWARD_DECL_STRONG(AM3BoardScheme)
FORWARD_DECL_STRONG(AM3CellScheme)
FORWARD_DECL_STRONG(AM3CellAppointmentScheme)

UCLASS()
class M3_API AM3App : public AActor
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

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3TapGestureRecognizerComponent* TapGestureRecognizerComponent;

	UPROPERTY(EditAnywhere, Category = "M3CustomInput")
	class UM3PanGestureRecognizerComponent* PanGestureRecognizerComponent;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UM3BoardAssetsBundle> AssetsBundle_BP;

	UPROPERTY(Category = "M3Bundle", EditAnywhere, BlueprintReadWrite)
	UM3BoardAssetsBundle* AssetsBundle;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AM3BoardScheme> BoardScheme_BP;

	UPROPERTY(Category = "M3Scheme", EditAnywhere, BlueprintReadWrite)
	AM3BoardScheme* BoardScheme;
};
