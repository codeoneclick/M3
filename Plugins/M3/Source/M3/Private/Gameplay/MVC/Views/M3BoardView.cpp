// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BoardView.h"
#include "EngineUtils.h"
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
	std::shared_ptr<M3AppEventModelProp_Callback> OnStateChangedCallback = std::make_shared<M3AppEventModelProp_Callback>(std::bind(&M3BoardView::OnStateChanged, this, std::placeholders::_1, std::placeholders::_2));
	OnStateChangedEvent = std::make_shared<M3AppEventModelProp>(M3ElementModel::ClassGuid(), M3ElementEntity::PROPERTY_ID_State(), OnStateChangedCallback);
	M3GlobalDispatcher::GetInstance()->Subscribe(OnStateChangedEvent);
}

M3BoardView::~M3BoardView() {
	if (OnStateChangedEvent) {
		M3GlobalDispatcher::GetInstance()->Unsubscribe(OnStateChangedEvent);
	}
}

void M3BoardView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);
}

void M3BoardView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& BoardEntity = BoardModel->Entity->Get();

	std::shared_ptr<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> CellsContainerSlot = std::make_shared<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>();
	Slots[k_ON_CELLS_CONTAINER_CHANGED] = CellsContainerSlot;
	const auto CellContainer = M3CellModel::Container();
	CellsContainerSlot->Attach(CellContainer, [=](const std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>& Value) {
		for (const auto& It : *Value.get()) {
			const auto CellModel = std::static_pointer_cast<M3CellModel>(It);
			if (!CellModel->Entity->Get()->IsAssignedToView->Get()) {
				AM3Cell* Cell = GetBundle<UM3BoardAssetsBundle>()->ConstructCell(GetSuperview()->GetWorld());
				Cell->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
				Cell->OnLoad(Bundle);
				Cell->OnBindViewModel(CellModel);
				Cell->OnBindViewDelegate();
				Cell->OnBindViewAccessor();
				AddSubview(Cell->GetView());

				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				Cells[Index] = Cell;
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> ElementsContainerSlot = std::make_shared<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>();
	Slots[k_ON_ELEMENTS_CONTAINER_CHANGED] = ElementsContainerSlot;
	ElementsContainerSlot->Attach(M3ElementModel::Container(), [=](const std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>& Value) {
		for (const auto& It : *Value.get()) {
			const auto ElementModel = std::static_pointer_cast<M3ElementModel>(It);
			if (!ElementModel->Entity->Get()->IsAssignedToView->Get()) {
				AM3Element* Element = GetBundle<UM3BoardAssetsBundle>()->ConstructElement(GetSuperview()->GetWorld());
				Element->AttachToActor(GetSuperview(), FAttachmentTransformRules::KeepWorldTransform);
				Element->OnLoad(Bundle);
				Element->OnBindViewModel(ElementModel);
				Element->OnBindViewDelegate();
				Element->OnBindViewAccessor();
				AddSubview(Element->GetView());

				const auto CellModel = ElementModel->GetParent<M3CellModel>();
				ensure(CellModel != nullptr);
				size_t Index = CellModel->GetCol() + CellModel->GetRow() * BoardModel->GetCols();
				Elements[Index] = Element;
			}
		}
	});

	std::shared_ptr<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> RegularElementsContainerSlot = std::make_shared<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>();
	Slots[k_ON_REGULAR_ELEMENTS_CONTAINER_CHANGED] = RegularElementsContainerSlot;
	RegularElementsContainerSlot->Attach(M3RegularElementModel::Container(), [=](const std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>& Value) {
		for (const auto& It : *Value.get()) {
			const auto RegularElementModel = std::static_pointer_cast<M3RegularElementModel>(It);
			if (!RegularElementModel->Entity->Get()->IsAssignedToView->Get()) {
				AM3RegularElement* RegularElement = GetBundle<UM3BoardAssetsBundle>()->ConstructRegularElement(GetSuperview()->GetWorld());
				RegularElement->OnLoad(Bundle);
				RegularElement->OnBindViewModel(RegularElementModel);
				RegularElement->OnBindViewDelegate();
				RegularElement->OnBindViewAccessor();

				const auto ElementModel = RegularElementModel->GetParent<M3ElementModel>();
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

	std::shared_ptr<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> SuperElementsContainerSlot = std::make_shared<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>();
	Slots[k_ON_SUPER_ELEMENTS_CONTAINER_CHANGED] = SuperElementsContainerSlot;
	SuperElementsContainerSlot->Attach(M3SuperElementModel::Container(), [=](const std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>& Value) {
		for (const auto& It : *Value.get()) {
			const auto SuperElementModel = std::static_pointer_cast<M3SuperElementModel>(It);
			if (!SuperElementModel->Entity->Get()->IsAssignedToView->Get()) {
				AM3SuperElement* SuperElement = GetBundle<UM3BoardAssetsBundle>()->ConstructSuperElement(GetSuperview()->GetWorld());
				SuperElement->OnLoad(Bundle);
				SuperElement->OnBindViewModel(SuperElementModel);
				SuperElement->OnBindViewDelegate();
				SuperElement->OnBindViewAccessor();

				const auto ElementModel = SuperElementModel->GetParent<M3ElementModel>();
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

	std::shared_ptr<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> BlockersContainerSlot = std::make_shared<M3KVMultiSlot<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>();
	Slots[k_ON_BLOCKERS_CONTAINER_CHANGED] = BlockersContainerSlot;
	BlockersContainerSlot->Attach(M3BlockerModel::Container(), [=](const std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>& Value) {
		for (const auto& It : *Value.get()) {
			const auto BlockerModel = std::static_pointer_cast<M3BlockerModel>(It);
			if (!BlockerModel->Entity->Get()->IsAssignedToView->Get()) {
				AM3Blocker* Blocker = GetBundle<UM3BoardAssetsBundle>()->ConstructBlocker(GetSuperview()->GetWorld());
				Blocker->OnLoad(Bundle);
				Blocker->OnBindViewModel(BlockerModel);
				Blocker->OnBindViewDelegate();
				Blocker->OnBindViewAccessor();

				const auto ElementModel = BlockerModel->GetParent<M3ElementModel>();
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

		int Cols = BoardModel->GetCols();
		int Rows = BoardModel->GetRows();

		Cells.resize(Cols * Rows, nullptr);
		Elements.resize(Cols * Rows, nullptr);
	});

	SizeSlot->Attach(BoardEntity->Rows, [=](const int& Value) {
		FVector CurrentLocation = GetSuperview()->GetActorLocation();
		float LocationX = -Value * BoardSettingsModel->GetElementSize().X * 0.5 + BoardSettingsModel->GetElementSize().X * 0.5;
		GetSuperview()->SetActorLocation(FVector(LocationX, CurrentLocation.Y, CurrentLocation.Z));

		int Cols = BoardModel->GetCols();
		int Rows = BoardModel->GetRows();

		Cells.resize(Cols * Rows, nullptr);
		Elements.resize(Cols * Rows, nullptr);
	});
}

void M3BoardView::OnStateChanged(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {

}

AM3Cell* M3BoardView::GetCell(int Col, int Row) {
	AM3Cell* Cell = nullptr;
	const auto& BoardModel = GetViewModel<M3BoardModel>();
	size_t Index = Col + Row * BoardModel->GetCols();
	if (Index < Cells.size()) {
		Cell = Cells[Index];
	}
	return Cell;
}

AM3Element* M3BoardView::GetElement(int Col, int Row) {
	AM3Element* Element = nullptr;
	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& ElementModel = BoardModel->GetElement(Col, Row);
	for (TActorIterator<AActor> It(GetSuperview()->GetWorld(), AM3Element::StaticClass()); It; ++It) {
		if (Cast<AM3Element>(*It)->GetModel() == ElementModel) {
			Element = Cast<AM3Element>(*It);
		}
	}
	return Element;
}

AM3Blocker* M3BoardView::GetBlocker(int Col, int Row) {
	AM3Blocker* Blocker = nullptr;
	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& ElementModel = BoardModel->GetElement(Col, Row);
	const auto& BlockerModel = ElementModel->GetSubmodel<M3BlockerModel>();
	for (TActorIterator<AActor> It(GetSuperview()->GetWorld(), AM3Blocker::StaticClass()); It; ++It) {
		if (Cast<AM3Blocker>(*It)->GetModel() == BlockerModel) {
			Blocker = Cast<AM3Blocker>(*It);
		}
	}
	return Blocker;
}

AM3SuperElement* M3BoardView::GetSuperElement(int Col, int Row) {
	AM3SuperElement* SuperElement = nullptr;
	const auto& BoardModel = GetViewModel<M3BoardModel>();
	const auto& ElementModel = BoardModel->GetElement(Col, Row);
	const auto& SuperElementModel = ElementModel->GetSubmodel<M3SuperElementModel>();
	for (TActorIterator<AActor> It(GetSuperview()->GetWorld(), AM3SuperElement::StaticClass()); It; ++It) {
		if (Cast<AM3SuperElement>(*It)->GetModel() == SuperElementModel) {
			SuperElement = Cast<AM3SuperElement>(*It);
		}
	}
	return SuperElement;
}