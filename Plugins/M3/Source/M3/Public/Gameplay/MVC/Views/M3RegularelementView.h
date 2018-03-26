// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3ElementView.h"
#include "M3Scheme.h"

FORWARD_DECL_STRONG(UM3ElementViewAccessor)

class M3_API M3RegularelementView : public M3ElementView {
protected:

	void SetElementVisual(EM3ElementId Id);

public:

	CTTI_CLASS_GUID(M3RegularelementView, M3View_INTERFACE::GuidsContainer)

	M3RegularelementView(AActor* _Superview);
	~M3RegularelementView();

	void Load(UM3AssetsBundle* Bundle) override;
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) override;
};
