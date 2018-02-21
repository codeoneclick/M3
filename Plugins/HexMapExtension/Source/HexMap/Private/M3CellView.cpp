// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CellView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"

M3CellView::M3CellView(AActor* Superview) : M3View(Superview)
{

}

void M3CellView::Load(AM3AssetsBundle* Bundle) {
	M3View::Load(Bundle);
}

void M3CellView::BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) {
	M3View::BindViewModel(ViewModel);
}
