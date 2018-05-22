// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3SuperElementVisualComponent.h"
#include "M3SuperElement.h"
#include "M3SuperElementModel.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UM3SuperElementVisualComponent::UM3SuperElementVisualComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3SuperElementVisualComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3SuperElementVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto SuperElement = Cast<AM3SuperElement>(GetOwner());
	const auto SuperElementModel = std::static_pointer_cast<M3SuperElementModel>(SuperElement->GetModel());
	const auto ExplosionRadius = M3SuperElementModel::GetExplosionRadius(SuperElementModel->GetId());

	const auto MeshComponent = Cast<UStaticMeshComponent>(SuperElement->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	UMaterialInstanceDynamic* MutableMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	if (MutableMaterial) {
		MutableMaterial->SetScalarParameterValue(FName(TEXT("GridSpacing")), 1);
	}

	const auto TextComponent = Cast<UTextRenderComponent>(SuperElement->GetComponentByClass(UTextRenderComponent::StaticClass()));
	if (TextComponent) {
		TextComponent->SetText(FString::Printf(TEXT("BOMB %s"), *FString::FromInt(ExplosionRadius + 1)));
	}
	
}

