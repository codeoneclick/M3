// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3Scheme.h"

FORWARD_DECL_STRONG(UM3ElementViewAccessor)

class M3_API M3RegularElementView : public M3View {
protected:

	void SetElementVisual(EM3ElementId Id);

public:

	CTTI_CLASS_GUID(M3RegularElementView)

	M3RegularElementView(AActor* _Superview);
	~M3RegularElementView();

	void Load(UM3AssetsBundle* _Bundle) override;
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) override;
};
