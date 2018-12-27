// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3CellView.h"
#include "M3KVSlot.h"
#include "M3CellModel.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"
#include "M3SharedModel.h"
#include "M3BoardSettingsModel.h"

M3CellView::M3CellView(AActor* _Superview) : M3View(_Superview) {

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

	const auto BoardSettingsModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardSettingsModel>();

	const auto& CellModel = GetViewModel<M3CellModel>();
	int Col = CellModel->Entity->Get()->Col->Get();
	int Row = CellModel->Entity->Get()->Row->Get();
	GetSuperview()->SetActorRelativeLocation(FVector(Row * BoardSettingsModel->GetElementSize().Y, Col * BoardSettingsModel->GetElementSize().X, 0));
	GetSuperview()->SetActorHiddenInGame(CellModel->GetIsClosed());
}
