// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardView.h"
#include "M3AssetsBundle.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3KVMultiSlot.h"
#include "M3Element.h"
#include "M3Cell.h"
#include "Engine/World.h"
#include "M3SharedModel.h"
#include "M3BoardSettingsModel.h"

FORWARD_DECL_CONTAINER(M3CellModel, M3Model<M3CellEntity>)
FORWARD_DECL_CONTAINER(M3ElementModel, M3Model<M3ElementEntity>)

const std::string k_ON_CELLS_CONTAINER_CHANGED = "ON_CELLS_CONTAINER_CHANGED";
const std::string k_ON_ELEMENTS_CONTAINER_CHANGED = "ON_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_SIZE_CHANGED = "ON_SIZE_CHANGED";

M3BoardView::M3BoardView(AActor* _Superview) : M3View(_Superview) {

}

M3BoardView::~M3BoardView() {

}

void M3BoardView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3BoardView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& BoardEntity = BoardModel->Entity->Get();

	std::shared_ptr<M3KVMultiSlot<M3CellModel_Container>> CellsContainerSlot = std::make_shared<M3KVMultiSlot<M3CellModel_Container>>();
	Slots[k_ON_CELLS_CONTAINER_CHANGED] = CellsContainerSlot;
	CellsContainerSlot->Attach(M3CellModel::Container, [=](const M3CellModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Cell* Cell = GetBundle<UM3BoardAssetsBundle>()->ConstructCell(GetSuperview()->GetWorld());
			Cell->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
			Cell->OnLoad(Bundle);
			Cell->OnBindViewModel(It);
			Cell->OnBindViewDelegate();
			AddSubview(Cell->GetView());
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3ElementModel_Container>> ElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3ElementModel_Container>>();
	Slots[k_ON_ELEMENTS_CONTAINER_CHANGED] = ElementsContainerSlot;
	ElementsContainerSlot->Attach(M3ElementModel::Container, [=](const M3ElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Element* Element = GetBundle<UM3BoardAssetsBundle>()->ConstructElement(GetSuperview()->GetWorld());
			Element->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
			Element->OnLoad(Bundle);
			Element->OnBindViewModel(It);
			Element->OnBindViewDelegate();
			AddSubview(Element->GetView());
		}
	});

	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	std::shared_ptr<M3KVMultiSlot<int>> SizeSlot = std::make_shared<M3KVMultiSlot<int>>();
	Slots[k_ON_SIZE_CHANGED] = SizeSlot;
	SizeSlot->Attach(BoardEntity->Cols, [=](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationY = -Value * BoardSettingsModel->GetElementSize().Y * 0.5;
		GetSuperview()->SetActorLocation(FVector(CurrentLocation.X, LocationY, CurrentLocation.Z));
	});

	SizeSlot->Attach(BoardEntity->Rows, [=](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationX = -Value * BoardSettingsModel->GetElementSize().X * 0.5;
		GetSuperview()->SetActorLocation(FVector(LocationX, CurrentLocation.Y, CurrentLocation.Z));
	});
}