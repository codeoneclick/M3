// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardView.h"
#include "M3AssetsBundle.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3RegularElementModel.h"
#include "M3SuperelementModel.h"
#include "M3KVMultiSlot.h"
#include "M3Element.h"
#include "M3Regularelement.h"
#include "M3Superelement.h"
#include "M3Cell.h"
#include "Engine/World.h"
#include "M3SharedModel.h"
#include "M3BoardSettingsModel.h"

FORWARD_DECL_LIST_CONTAINER(M3CellModel, M3Model<M3CellEntity>)
FORWARD_DECL_LIST_CONTAINER(M3ElementModel, M3Model<M3ElementEntity>)
FORWARD_DECL_LIST_CONTAINER(M3RegularelementModel, M3Model<M3RegularelementEntity>)
FORWARD_DECL_LIST_CONTAINER(M3SuperelementModel, M3Model<M3SuperelementEntity>)

const std::string k_ON_CELLS_CONTAINER_CHANGED = "ON_CELLS_CONTAINER_CHANGED";
const std::string k_ON_ELEMENTS_CONTAINER_CHANGED = "ON_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_REGULAR_ELEMENTS_CONTAINER_CHANGED = "ON_REGULAR_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_SUPER_ELEMENTS_CONTAINER_CHANGED = "ON_SUPER_ELEMENTS_CONTAINER_CHANGED";
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
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3Cell* Cell = GetBundle<UM3BoardAssetsBundle>()->ConstructCell(GetSuperview()->GetWorld());
				Cell->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
				Cell->OnLoad(Bundle);
				Cell->OnBindViewModel(It);
				Cell->OnBindViewDelegate();
				AddSubview(Cell->GetView());

				const auto CellModel = std::static_pointer_cast<M3CellModel>(It);
				int Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				if (Cells.size() <= Index) {
					Cells.resize(Index + 1);
				}
				Cells[Index] = Cell;
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3ElementModel_Container>> ElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3ElementModel_Container>>();
	Slots[k_ON_ELEMENTS_CONTAINER_CHANGED] = ElementsContainerSlot;
	ElementsContainerSlot->Attach(M3ElementModel::Container, [=](const M3ElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3Element* Element = GetBundle<UM3BoardAssetsBundle>()->ConstructElement(GetSuperview()->GetWorld());
				Element->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
				Element->OnLoad(Bundle);
				Element->OnBindViewModel(It);
				Element->OnBindViewDelegate();
				AddSubview(Element->GetView());

				const auto CellModel = It->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);
				int Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				if (Elements.size() <= Index) {
					Elements.resize(Index + 1);
				}
				Elements[Index] = Element;
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3RegularelementModel_Container>> RegularElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3RegularelementModel_Container>>();
	Slots[k_ON_REGULAR_ELEMENTS_CONTAINER_CHANGED] = RegularElementsContainerSlot;
	RegularElementsContainerSlot->Attach(M3RegularelementModel::Container, [=](const M3RegularelementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3Regularelement* Regularelement = GetBundle<UM3BoardAssetsBundle>()->ConstructRegularelement(GetSuperview()->GetWorld());
				Regularelement->OnLoad(Bundle);
				Regularelement->OnBindViewModel(It);
				Regularelement->OnBindViewDelegate();

				const auto ElementModel = It->GetParent<M3ElementModel>();
				ensure(ElementModel != nullptr);
				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);

				int Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				ensure(Index < Elements.size());
				AM3Element* Element = Elements[Index];
				Element->GetView()->AddSubview(Regularelement->GetView());
				Regularelement->AttachToActor(Element, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3SuperelementModel_Container>> SuperElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3SuperelementModel_Container>>();
	Slots[k_ON_SUPER_ELEMENTS_CONTAINER_CHANGED] = SuperElementsContainerSlot;
	SuperElementsContainerSlot->Attach(M3SuperelementModel::Container, [=](const M3SuperelementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3Superelement* Superelement = GetBundle<UM3BoardAssetsBundle>()->ConstructSuperelement(GetSuperview()->GetWorld());
				Superelement->OnLoad(Bundle);
				Superelement->OnBindViewModel(It);
				Superelement->OnBindViewDelegate();

				const auto ElementModel = It->GetParent<M3ElementModel>();
				ensure(ElementModel != nullptr);
				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);
				UE_LOG(LogTemp, Warning, TEXT("Superelement view added at cell %d %d"), CellModel->GetCol(), CellModel->GetRow());

				int Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				ensure(Index < Elements.size());
				AM3Element* Element = Elements[Index];
				Element->GetView()->AddSubview(Superelement->GetView());
				Superelement->AttachToActor(Element, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	});

	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	std::shared_ptr<M3KVMultiSlot<int>> SizeSlot = std::make_shared<M3KVMultiSlot<int>>();
	Slots[k_ON_SIZE_CHANGED] = SizeSlot;
	SizeSlot->Attach(BoardEntity->Cols, [=](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationY = -Value * BoardSettingsModel->GetElementSize().Y * 0.5 + BoardSettingsModel->GetElementSize().Y * 0.5;
		GetSuperview()->SetActorLocation(FVector(CurrentLocation.X, LocationY, CurrentLocation.Z));
	});

	SizeSlot->Attach(BoardEntity->Rows, [=](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationX = -Value * BoardSettingsModel->GetElementSize().X * 0.5 + BoardSettingsModel->GetElementSize().X * 0.5;
		GetSuperview()->SetActorLocation(FVector(LocationX, CurrentLocation.Y, CurrentLocation.Z));
	});
}