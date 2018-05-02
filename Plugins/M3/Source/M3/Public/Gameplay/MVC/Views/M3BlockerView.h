// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3Scheme.h"

class M3_API M3BlockerView : public M3View {
protected:

	void SetElementVisual(EM3ElementId Id);

public:

	CTTI_CLASS_GUID(M3BlockerView)

	M3BlockerView(AActor* _Superview);
	~M3BlockerView();

	void Load(UM3AssetsBundle* Bundle) override;
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) override;
};
