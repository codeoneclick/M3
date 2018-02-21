// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardView.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3KVMultiSlot.h"
#include "M3Element.h"
#include "M3Cell.h"
#include "Engine/World.h"

FORWARD_DECL_CONTAINER(M3CellModel, M3Model<M3CellEntity>)
FORWARD_DECL_CONTAINER(M3ElementModel, M3Model<M3ElementEntity>)

const std::string k_ON_CELLS_CONTAINER_CHANGED = "ON_CELLS_CONTAINER_CHANGED";
const std::string k_ON_ELEMENTS_CONTAINER_CHANGED = "ON_ELEMENTS_CONTAINER_CHANGED";

M3BoardView::M3BoardView(AActor* Superview) : M3View(Superview)
{

}

void M3BoardView::Load(AM3AssetsBundle* Bundle) {
	M3View::Load(Bundle);
}

void M3BoardView::BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) {
	M3View::BindViewModel(ViewModel);

	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& BoardEntity = BoardModel->Entity->Get();

	std::shared_ptr<M3KVMultiSlot<M3CellModel_Container>> CellsContainerSlot = std::make_shared<M3KVMultiSlot<M3CellModel_Container>>();
	Slots[k_ON_CELLS_CONTAINER_CHANGED] = CellsContainerSlot;
	CellsContainerSlot->Attach(M3CellModel::Container, [this](const M3CellModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Cell* Cell = GetSuperview()->GetWorld()->SpawnActor<AM3Cell>(FVector(0, 0, 0), FRotator(0, 0, 0));
			Cell->OnLoad(Bundle);
			Cell->OnBindViewModel(It);
			AddSubview(Cell->GetView());
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3ElementModel_Container>> ElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3ElementModel_Container>>();
	Slots[k_ON_ELEMENTS_CONTAINER_CHANGED] = ElementsContainerSlot;
	ElementsContainerSlot->Attach(M3ElementModel::Container, [this](const M3ElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			AM3Element* Element = GetSuperview()->GetWorld()->SpawnActor<AM3Element>(FVector(0, 0, 0), FRotator(0, 0, 0));
			Element->OnLoad(Bundle);
			Element->OnBindViewModel(It);
			AddSubview(Element->GetView());
		}
	});
}