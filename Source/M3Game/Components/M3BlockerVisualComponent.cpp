// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3BlockerVisualComponent.h"
#include "M3Blocker.h"
#include "M3BlockerModel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UM3BlockerVisualComponent::UM3BlockerVisualComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UM3BlockerVisualComponent::BeginPlay() {
	Super::BeginPlay();
}

void UM3BlockerVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto Blocker = Cast<AM3Blocker>(GetOwner());

	const auto BlockerModel = std::static_pointer_cast<M3BlockerModel>(Blocker->GetModel());
	const auto MeshComponent = Cast<UStaticMeshComponent>(Blocker->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	const auto TextComponent = Cast<UTextRenderComponent>(Blocker->GetComponentByClass(UTextRenderComponent::StaticClass()));
	UMaterialInstanceDynamic* MutableMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	if (MutableMaterial) {
		switch (BlockerModel->GetId()) {
		case EM3ElementId::BLOCKER_BOX1X:
		case EM3ElementId::BLOCKER_BOX2X:
		case EM3ElementId::BLOCKER_BOX3X:
		{
			MutableMaterial->SetScalarParameterValue(FName(TEXT("GridSpacing")), 1);
			MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(.25f, .25f, .25f, 1.f));
			if (TextComponent) {
				TextComponent->SetText(FString::Printf(TEXT("BOX %s"), *FString::FromInt(BlockerModel->GetDensity())));
			}
		}
		break;

		case EM3ElementId::BLOCKER_WIRE1X:
		case EM3ElementId::BLOCKER_WIRE2X:
		case EM3ElementId::BLOCKER_ICE1X:
		case EM3ElementId::BLOCKER_ICE2X:
		{
			const auto ElementModel = BlockerModel->GetParent<M3ElementModel>();

			switch (ElementModel->GetColor()) {
			case EM3ElementColor::RED:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			break;

			case EM3ElementColor::GREEN:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.f, 1.f, 0.f, 1.f));
			}
			break;

			case EM3ElementColor::BLUE:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.f, 0.f, 1.f, 1.f));
			}
			break;

			case EM3ElementColor::YELLOW:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.f, 1.f, 0.f, 1.f));
			}
			break;

			case EM3ElementColor::ORANGE:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.f, .25f, 0.f, 1.f));
			}
			break;

			case EM3ElementColor::PURPLE:
			{
				MutableMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.f, 0.f, 1.f, 1.f));
			}
			break;
			default:
				break;
			}
			MutableMaterial->SetScalarParameterValue(FName(TEXT("GridSpacing")), 1);

			if (TextComponent) {
				if (BlockerModel->GetId() == EM3ElementId::BLOCKER_ICE1X ||
					BlockerModel->GetId() == EM3ElementId::BLOCKER_ICE2X) {
					TextComponent->SetText(FString::Printf(TEXT("ICE %s"), *FString::FromInt(BlockerModel->GetDensity())));
				} else {
					TextComponent->SetText(FString::Printf(TEXT("WIRE %s"), *FString::FromInt(BlockerModel->GetDensity())));
				}
			}
		}
		break;

		default:
			break;
		}
	}
}

