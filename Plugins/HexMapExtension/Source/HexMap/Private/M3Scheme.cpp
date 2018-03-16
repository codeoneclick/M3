// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Scheme.h"

#if WITH_EDITOR

#include "M3App.h"
#include "EngineUtils.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "EditorModeManager.h"
#include "M3AssetsBundle.h"

#endif

#if WITH_EDITOR

AM3CellAppointmentScheme* AM3CellScheme::EdModeSelectedAppointmentScheme = nullptr;

#endif

AM3CellScheme::AM3CellScheme() {
	PrimaryActorTick.bCanEverTick = true;
}

void AM3CellScheme::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);
}

void AM3CellScheme::AddAppointment(AM3CellAppointmentScheme* _Appointment) {
	bool bIsReplaced = false;
	for (int i = 0; i < Appointments.Num(); ++i) {
		const auto& Appointment = Appointments[i];
		if (Appointment->Appointment == _Appointment->Appointment) {
			bIsReplaced = true;
			Appointments[i] = _Appointment;
			break;
		}
	}
	if (!bIsReplaced) {
		Appointments.Add(_Appointment);
	}
}

AM3CellAppointmentScheme* AM3CellScheme::GetAppointment(EM3CellAppointment AppointmentId) const {
	AM3CellAppointmentScheme* Appointment = nullptr;

	for (const auto& It : Appointments) {
		if (It->Appointment == AppointmentId) {
			Appointment = It;
			break;
		}
	}

	return Appointment;
}

#if WITH_EDITOR

void AM3CellScheme::OnEditorTick(float DeltaTime) {

}

void AM3CellScheme::OnEditorMousePressed() {

}

void AM3CellScheme::OnEditorMouseReleased() {

	USelection* Selection = GEditor->GetSelectedActors();
	for (int32 i = 0; i < Selection->Num(); ++i) {
		UObject* Object = Selection->GetSelectedObject(i);
		if (Object && Object->IsA(AM3CellScheme::StaticClass()) && Object == this) {
			if (AM3CellScheme::EdModeSelectedAppointmentScheme) {
				UWorld* World = GEditor->GetEditorWorldContext().World();
				AM3App* M3App = nullptr;
				for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
					M3App = static_cast<AM3App*>(*It);
					break;
				}
				if (M3App) {
					const auto BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(M3App->AssetsBundle);
					Appointments.Empty();
					AddAppointment(AM3CellScheme::EdModeSelectedAppointmentScheme);

					UStaticMeshComponent* MeshComponent = nullptr;
					TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
					for (UActorComponent* ActorComponent : MeshesComponents) {
						MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
						break;
					}

					if (MeshComponent) {
						switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
						case EM3ElementId::ELEMENT_RED:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
							break;
						case EM3ElementId::ELEMENT_GREEN:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
							break;
						case EM3ElementId::ELEMENT_BLUE:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
							break;
						case EM3ElementId::ELEMENT_YELLOW:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
							break;
						case EM3ElementId::ELEMENT_ORANGE:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
							break;
						case EM3ElementId::ELEMENT_PURPLE:
							MeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
							MeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
							break;
						default:
							break;
						}
					}
					GEditor->SelectNone(true, true);
				}
			}
		}
	}
}

#endif