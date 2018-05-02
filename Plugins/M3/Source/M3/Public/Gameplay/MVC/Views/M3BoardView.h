// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3Utilities.h"
#include "M3View.h"

FORWARD_DECL_STRONG(M3AppEventModelProp)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)
FORWARD_DECL_STRONG(AM3Cell)
FORWARD_DECL_STRONG(AM3Element)
FORWARD_DECL_STRONG(AM3RegularElement)
FORWARD_DECL_STRONG(AM3SuperElement)
FORWARD_DECL_STRONG(AM3Blocker)

class M3_API M3BoardView : public M3View {

protected:

	std::vector<AM3Cell *> Cells;
	std::vector<AM3Element *> Elements;

	M3AppEventModelProp_SharedPtr OnStateChangedEvent = nullptr;
	void OnStateChanged(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop);

public:	

	M3BoardView(AActor* _Superview);
	~M3BoardView();

	CTTI_CLASS_GUID(M3BoardView)

	void Load(UM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);

	AM3Cell* GetCell(int Col, int Row);
	AM3Element* GetElement(int Col, int Row);
};
