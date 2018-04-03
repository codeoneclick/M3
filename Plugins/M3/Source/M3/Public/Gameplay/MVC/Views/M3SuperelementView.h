// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3Scheme.h"

FORWARD_DECL_STRONG(UM3ElementViewAccessor)

class M3_API M3SuperelementView : public M3View {
protected:

	void SetSuperelementVisual(EM3SuperelementId Id);
	
public:

	CTTI_CLASS_GUID(M3SuperelementView, M3View_INTERFACE::GuidsContainer)

	M3SuperelementView(AActor* _Superview);
	~M3SuperelementView();

	void Load(UM3AssetsBundle* Bundle) override;
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) override;
};
