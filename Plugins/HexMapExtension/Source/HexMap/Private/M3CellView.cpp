// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CellView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"

M3CellView::M3CellView(UM3ViewFactory* _ViewFactory, AActor* _Superview) : M3View(_ViewFactory, _Superview)
{

}

void M3CellView::Load(UM3AssetsBundle* _Bundle) {
	M3View::Load(_Bundle);

	UM3BoardAssetsBundle* BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(_Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		MeshComponent->SetStaticMesh(BoardAssetsBundle->Cell.Mesh);
		MeshComponent->SetMaterial(0, BoardAssetsBundle->Cell.Material);
	}
}

void M3CellView::BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) {
	M3View::BindViewModel(_ViewModel);

	const auto& CellModel = GetViewModel<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();
	GetSuperview()->SetActorRelativeLocation(FVector(Col * 110, Row * 110, 0));
}
