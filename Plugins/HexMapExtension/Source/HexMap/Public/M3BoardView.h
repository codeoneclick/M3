// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M3View.h"

class HEXMAP_API M3BoardView : public M3View
{
	
public:	

	M3BoardView(UM3ViewFactory* _ViewFactory, AActor* _Superview);
	~M3BoardView();

	CTTI_CLASS_GUID(M3BoardView, M3View_INTERFACE::GuidsContainer)

	void Load(UM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
