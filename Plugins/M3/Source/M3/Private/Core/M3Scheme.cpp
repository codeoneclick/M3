// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Scheme.h"
#include "M3App.h"
#include "M3AssetsBundle.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

#if WITH_EDITOR

#include "EngineUtils.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "EditorModeManager.h"

#endif

AM3CellAppointmentScheme* AM3CellScheme::EdModeSelectedAppointmentScheme = nullptr;

AM3CellScheme::AM3CellScheme() {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellClosedMesh_RESOURCE(TEXT("StaticMesh'/M3/M3_SM_CellClosed.M3_SM_CellClosed'"));
	CellClosedMesh = CellClosedMesh_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellHoleMesh_RESOURCE(TEXT("StaticMesh'/M3/M3_SM_CellHole.M3_SM_CellHole'"));
	CellHoleMesh = CellHoleMesh_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellRandomMesh_RESOURCE(TEXT("StaticMesh'/M3/M3_SM_CellRandom.M3_SM_CellRandom'"));
	CellRandomMesh = CellRandomMesh_RESOURCE.Get();

	static ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> CellClosedMaterial_RESOURCE(TEXT("MaterialInstanceConstant'/M3/M3_MI_BLACK.M3_MI_BLACK'"));
	CellClosedMaterial = CellClosedMaterial_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> CellHoleMaterial_RESOURCE(TEXT("MaterialInstanceConstant'/M3/M3_MI_GRAY.M3_MI_GRAY'"));
	CellHoleMaterial = CellHoleMaterial_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> CellRandomMaterial_RESOURCE(TEXT("MaterialInstanceConstant'/M3/M3_MI_PURPLE.M3_MI_PURPLE'"));
	CellRandomMaterial = CellRandomMaterial_RESOURCE.Get();
}

void AM3CellScheme::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);

	UStaticMeshComponent* MeshComponent = nullptr;
	TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

bool AM3CellScheme::IsAppointmentExist(EM3CellAppointment AppointmentId) const {
	return AM3CellScheme::GetAppointment(AppointmentId) != nullptr;
}

#if WITH_EDITOR

void AM3CellScheme::PostEditChangeProperty(struct FPropertyChangedEvent& Event) {
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != nullptr) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AM3CellScheme, Col) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(AM3CellScheme, Row)) {
	}
}

void AM3CellScheme::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) {
	// Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
}

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
						if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::REGULARELEMENT) {
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
						} else if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::FUNCTIONAL) {
							switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
							case EM3ElementId::CELL_CLOSED:
								MeshComponent->SetStaticMesh(CellClosedMesh);
								MeshComponent->SetMaterial(0, CellClosedMaterial);
								break;
							case EM3ElementId::CELL_HOLE:
								MeshComponent->SetStaticMesh(CellHoleMesh);
								MeshComponent->SetMaterial(0, CellHoleMaterial);
								break;
							case EM3ElementId::CELL_RANDOM:
								MeshComponent->SetStaticMesh(CellRandomMesh);
								MeshComponent->SetMaterial(0, CellRandomMaterial);
								break;
							default:
								break;
							}
						}
					}
					GEditor->SelectNone(true, true);
				}
			}
		}
	}
}

#endif

AM3GoalScheme* AM3BoardScheme::GetGoalScheme(UWorld* World, EM3GoalId Id) {
	AM3GoalScheme* GoalScheme = nullptr;
	for (const auto Goal : Goals) {
		if (Goal->Id == Id) {
			GoalScheme = Goal;
			break;
		}
	}
	if (!GoalScheme) {
		GoalScheme = World->SpawnActor<AM3GoalScheme>(FVector(0.f), FRotator(0.f));
		GoalScheme->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		GoalScheme->Id = Id;
		GoalScheme->Quantity = 0;
		GoalScheme->Enabled = false;
		Goals.Add(GoalScheme);
	}
	return GoalScheme;
}