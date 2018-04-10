// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardView.h"
#include "M3AssetsBundle.h"
#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"
#include "M3BlockerModel.h"
#include "M3RegularElementModel.h"
#include "M3SuperElementModel.h"
#include "M3KVMultiSlot.h"
#include "M3Element.h"
#include "M3RegularElement.h"
#include "M3SuperElement.h"
#include "M3Blocker.h"
#include "M3Cell.h"
#include "Engine/World.h"
#include "M3SharedModel.h"
#include "M3BoardSettingsModel.h"

FORWARD_DECL_LIST_CONTAINER(M3CellModel, M3Model<M3CellEntity>)
FORWARD_DECL_LIST_CONTAINER(M3ElementModel, M3Model<M3ElementEntity>)
FORWARD_DECL_LIST_CONTAINER(M3RegularElementModel, M3Model<M3RegularElementEntity>)
FORWARD_DECL_LIST_CONTAINER(M3SuperElementModel, M3Model<M3SuperElementEntity>)
FORWARD_DECL_LIST_CONTAINER(M3BlockerModel, M3Model<M3BlockerEntity>)

const std::string k_ON_CELLS_CONTAINER_CHANGED = "ON_CELLS_CONTAINER_CHANGED";
const std::string k_ON_ELEMENTS_CONTAINER_CHANGED = "ON_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_REGULAR_ELEMENTS_CONTAINER_CHANGED = "ON_REGULAR_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_SUPER_ELEMENTS_CONTAINER_CHANGED = "ON_SUPER_ELEMENTS_CONTAINER_CHANGED";
const std::string k_ON_BLOCKERS_CONTAINER_CHANGED = "ON_BLOCKERS_CONTAINER_CHANGED";
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
				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
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
				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				if (Elements.size() <= Index) {
					Elements.resize(Index + 1);
				}
				Elements[Index] = Element;
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3RegularElementModel_Container>> RegularElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3RegularElementModel_Container>>();
	Slots[k_ON_REGULAR_ELEMENTS_CONTAINER_CHANGED] = RegularElementsContainerSlot;
	RegularElementsContainerSlot->Attach(M3RegularElementModel::Container, [=](const M3RegularElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3RegularElement* RegularElement = GetBundle<UM3BoardAssetsBundle>()->ConstructRegularElement(GetSuperview()->GetWorld());
				RegularElement->OnLoad(Bundle);
				RegularElement->OnBindViewModel(It);
				RegularElement->OnBindViewDelegate();

				const auto ElementModel = It->GetParent<M3ElementModel>();
				ensure(ElementModel != nullptr);
				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);

				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				ensure(Index < Elements.size());
				AM3Element* Element = Elements[Index];
				Element->GetView()->AddSubview(RegularElement->GetView());
				RegularElement->AttachToActor(Element, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3SuperElementModel_Container>> SuperElementsContainerSlot = std::make_shared<M3KVMultiSlot<M3SuperElementModel_Container>>();
	Slots[k_ON_SUPER_ELEMENTS_CONTAINER_CHANGED] = SuperElementsContainerSlot;
	SuperElementsContainerSlot->Attach(M3SuperElementModel::Container, [=](const M3SuperElementModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3SuperElement* SuperElement = GetBundle<UM3BoardAssetsBundle>()->ConstructSuperElement(GetSuperview()->GetWorld());
				SuperElement->OnLoad(Bundle);
				SuperElement->OnBindViewModel(It);
				SuperElement->OnBindViewDelegate();

				const auto ElementModel = It->GetParent<M3ElementModel>();
				ensure(ElementModel != nullptr);
				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);

				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				ensure(Index < Elements.size());
				AM3Element* Element = Elements[Index];
				Element->GetView()->AddSubview(SuperElement->GetView());
				SuperElement->AttachToActor(Element, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<M3BlockerModel_Container>> BlockersContainerSlot = std::make_shared<M3KVMultiSlot<M3BlockerModel_Container>>();
	Slots[k_ON_BLOCKERS_CONTAINER_CHANGED] = BlockersContainerSlot;
	BlockersContainerSlot->Attach(M3BlockerModel::Container, [=](const M3BlockerModel_Container& Value) {
		for (const auto& It : *Value.get()) {
			if (!It->Entity->Get()->IsAssignedToView->Get()) {
				AM3Blocker* Blocker = GetBundle<UM3BoardAssetsBundle>()->ConstructBlocker(GetSuperview()->GetWorld());
				Blocker->OnLoad(Bundle);
				Blocker->OnBindViewModel(It);
				Blocker->OnBindViewDelegate();

				const auto ElementModel = It->GetParent<M3ElementModel>();
				ensure(ElementModel != nullptr);
				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);

				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				ensure(Index < Elements.size());
				AM3Element* Element = Elements[Index];
				Element->GetView()->AddSubview(Blocker->GetView());
				Blocker->AttachToActor(Element, FAttachmentTransformRules::KeepRelativeTransform);
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