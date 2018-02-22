// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CellView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"

M3CellView::M3CellView(AActor* Superview) : M3View(Superview)
{

}

void M3CellView::Load(AM3AssetsBundle* Bundle) {
	M3View::Load(Bundle);

	AM3BoardAssetsBundle* BoardAssetsBundle = static_cast<AM3BoardAssetsBundle*>(Bundle);

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		MeshComponent->SetStaticMesh(BoardAssetsBundle->Cell.Mesh);
		MeshComponent->SetMaterial(0, BoardAssetsBundle->Cell.Material);
	}
}

void M3CellView::BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) {
	M3View::BindViewModel(ViewModel);

	const auto& CellModel = GetViewModel<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();

	TArray<UActorComponent*> MeshesComponents = Superview->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		MeshComponent->SetWorldLocation(FVector(Col * 110, Row * 110, 0));
	}
}
