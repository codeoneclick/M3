// Fill out your copyright notice in the Description page of Project Settings.

#include "M3App.h"
#include "M3Board.h"
#include "M3CoordinatingComponent.h"
#include "M3AssetsBundle.h"
#include "M3Scheme.h"

AM3App::AM3App()
{
	PrimaryActorTick.bCanEverTick = true;
	CoordinatingComponent = CreateDefaultSubobject<UM3CoordinatingComponent>(TEXT("M3CoordinatingComponent"));
}

void AM3App::BeginPlay()
{
	Super::BeginPlay();
	CoordinatingComponent->CreateModels();
	CoordinatingComponent->CreateControllers();
	CoordinatingComponent->CreateViews(AssetsBundle);
	CoordinatingComponent->OnModelChanged(Scheme);
}

void AM3App::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

