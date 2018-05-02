// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"

class M3_API M3CellView : public M3View
{
public:

	M3CellView(AActor* Superview);

	CTTI_CLASS_GUID(M3CellView)

	void Load(UM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
