// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardView.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3KVMultiSlot.h"
#include "M3Element.h"
#include "M3Cell.h"
#include "M3ViewFactory.h"
#include "Engine/World.h"

FORWARD_DECL_CONTAINER(M3CellModel, M3Model<M3CellEntity>)
FORWARD_DECL_CONTAINER(M3ElementModel, M3Model<M3ElementEntity>)

const std::string k_ON_CELLS_CONTAINER_CHANGED = "ON_CELLS_CONTAINER_CHANGED";
const std::string k_ON_ELEMENTS_CONTAINER_CHANGED = "ON_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_SIZE_CHANGED = "ON_SIZE_CHANGED";

M3BoardView::M3BoardView(UM3ViewFactory* _ViewFactory, AActor* _Superview) : M3View(_ViewFactory, _Superview) {

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
	CellsContainerSlot->Attach(M3CellModel::Container, [this](const M3CellModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Cell* Cell = ViewFactory->CreateCell(GetSuperview()->GetWorld());
			Cell->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
			Cell->OnLoad(ViewFactory, Bundle);
			Cell->OnBindViewModel(It);
			Cell->OnBindViewDelegate();
			AddSubview(Cell->GetView());
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3ElementModel_Container>> ElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3ElementModel_Container>>();
	Slots[k_ON_ELEMENTS_CONTAINER_CHANGED] = ElementsContainerSlot;
	ElementsContainerSlot->Attach(M3ElementModel::Container, [this](const M3ElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Element* Element = ViewFactory->CreateElement(GetSuperview()->GetWorld());
			Element->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
			Element->OnLoad(ViewFactory, Bundle);
			Element->OnBindViewModel(It);
			Element->OnBindViewDelegate();
			AddSubview(Element->GetView());
		}
	});

	std::shared_ptr<M3KVMultiSlot<int>> SizeSlot = std::make_shared<M3KVMultiSlot<int>>();
	Slots[k_ON_SIZE_CHANGED] = SizeSlot;
	SizeSlot->Attach(BoardEntity->Cols, [this](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationX = -Value * 100 * 0.5;
		GetSuperview()->SetActorLocation(FVector(LocationX, CurrentLocation.Y, CurrentLocation.Z));
	});

	SizeSlot->Attach(BoardEntity->Rows, [this](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationY = -Value * 100 * 0.5;
		GetSuperview()->SetActorLocation(FVector(CurrentLocation.X, LocationY, CurrentLocation.Z));
	});
}