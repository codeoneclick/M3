// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3View.h"

class M3_API M3BoardView : public M3View
{
	
public:	

	M3BoardView(AActor* _Superview);
	~M3BoardView();

	CTTI_CLASS_GUID(M3BoardView, M3View_INTERFACE::GuidsContainer)

	void Load(UM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
