// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "M3GamePlayerController.h"
#include "EngineUtils.h"
#include "M3App.h"
#include "M3Board.h"
#include "M3BoardView.h"
#include "M3Cell.h"
#include "M3CellModel.h"
#include "M3Blocker.h"
#include "M3Element.h"
#include "M3ElementModel.h"
#include "M3SuperElementModel.h"
#include "M3BoardModel.h"
#include "M3SharedModel.h"
#include "MVC/Models/M3ExplosionModel.h"
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

	std::shared_ptr<M3AppEventModelProp_Callback> OnMatchingsCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&AM3GamePlayerController::OnMatching, this, std::placeholders::_1, std::placeholders::_2));
	OnMatchingEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnMatchingsCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnMatchingEvent);

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
	const auto ElementModel = std::static_pointer_cast<M3ElementModel>(Model);
	if (ElementModel->IsSuper() && ElementModel->IsInState(EM3ElementState::MATCHING)) {
		const auto BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();
		const auto Board = GetM3Board();
		const auto BoardView = std::static_pointer_cast<M3BoardView>(Board->GetView());

		const auto SuperElementModel = ElementModel->GetSubmodel<M3SuperElementModel>();
		ensure(SuperElementModel != nullptr);

		const auto CellModel = ElementModel->GetParent<M3CellModel>();
		ensure(CellModel != nullptr);

		int Col = CellModel->GetCol();
		int Row = CellModel->GetRow();
		int Radius = M3SuperElementModel::GetExplosionRadius(SuperElementModel->GetId());

		M3ElementModel_SharedPtr OtherElementModel = nullptr;
		for (int i = Col - Radius; i <= Col + Radius; ++i) {
			for (int j = Row - Radius; j <= Row + Radius; ++j) {
				if ((i == Col - Radius && j == Row - Radius) ||
					(i == Col - Radius && j == Row + Radius) ||
					(i == Col + Radius && j == Row - Radius) ||
					(i == Col + Radius && j == Row + Radius)) {
					continue;
				}
				OtherElementModel = BoardModel->GetElement(i, j);
				if (OtherElementModel && OtherElementModel->CanMatch()) {
					const auto OtherElement = BoardView->GetElement(i, j);
					if (OtherElement) {
						ensure(OtherElement->GetModel() == OtherElementModel);
						const auto OtherElementView = std::static_pointer_cast<M3ElementView>(OtherElement->GetView());
						const auto OtherElementViewAccessor = OtherElementView->GetAccessor<UM3ElementExplosionViewAccessor>();
						OtherElementViewAccessor->ExplosionCol = Col;
						OtherElementViewAccessor->ExplosionRow = Row;
						OtherElementViewAccessor->IsExplosion = true;
					}
				}
			}
		}
	}
}

AM3App* AM3GamePlayerController::GetM3App() const {
	UWorld* World = GetWorld();
	AM3App* M3App = nullptr;
	for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
		M3App = static_cast<AM3App*>(*It);
		break;
	}
	return M3App;
}

class AM3Board* AM3GamePlayerController::GetM3Board() const {
	UWorld* World = GetWorld();
	AM3Board* M3Board = nullptr;
	for (TActorIterator<AActor> It(World, AM3Board::StaticClass()); It; ++It) {
		M3Board = static_cast<AM3Board*>(*It);
		break;
	}
	return M3Board;
}

