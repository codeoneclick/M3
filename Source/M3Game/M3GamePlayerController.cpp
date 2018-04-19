// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "M3GamePlayerController.h"
#include "EngineUtils.h"
#include "M3App.h"
#include "M3Cell.h"
#include "M3Blocker.h"
#include "M3Element.h"
//#include "M3ElementModel.h"
#include "M3AppEvent.h"
#include "M3GlobalDispatcher.h"
#include "M3AssetsBundle.h"
#include "MVC/Views/M3ElementExplosionViewDelegate.h"
#include "MVC/Views/M3ElementExplosionViewAccessor.h"

AM3GamePlayerController::AM3GamePlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

AM3GamePlayerController::~AM3GamePlayerController() {
	if (OnMatchingEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnMatchingEvent);
	}
}

void AM3GamePlayerController::BeginPlay() {
	Super::BeginPlay();

	//std::shared_ptr<M3AppEventModelProp_Callback> OnMatchingsCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&AM3GamePlayerController::OnMatching, this, std::placeholders::_1, std::placeholders::_2));
	//OnMatchingEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnMatchingsCallback);
	//M3GlobalDispatcher::GetInstance()->Subscribe(OnMatchingEvent);

	UWorld* World = GetWorld();

	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		const auto App = Cast<AM3App>(*It);
		App->AssetsBundle->Element_BP = ElementExplosion_BP;
	}

	for (TActorIterator<AActor> It(World, AM3Cell::StaticClass()); It; ++It) {
		FVector Location = It->GetActorLocation();
		Location.Z -= 5.f;
		It->SetActorLocation(Location);
	}

	for (TActorIterator<AActor> It(World, AM3Blocker::StaticClass()); It; ++It) {
		FVector Location = It->GetActorLocation();
		Location.Z += 5.f;
		It->SetActorLocation(Location);
	}

	for (TActorIterator<AActor> It(World, AM3Element::StaticClass()); It; ++It) {
		const auto Element = Cast<AM3Element>(*It);
		if (ElementExplosion_BP) {
			Element->Delegate_BP = ElementExplosionViewDelegate_BP;
			Element->Accessor_BP = ElementExplosionViewAccessor_BP;
			Element->OnBindViewDelegate();
			Element->OnBindViewAccessor();
		}
	}
}

void AM3GamePlayerController::OnMatching(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
	//const auto ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	//if (ElementModel->IsSuper() && ElementModel->IsInState(EM3ElementState::MATCHING)) {

	//}
}
