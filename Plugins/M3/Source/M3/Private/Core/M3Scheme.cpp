// Copyright serhii serhiiv 2018 All rights reserved.

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

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellClosedMesh_RESOURCE(TEXT("StaticMesh'/M3/SM_M3_CellFunctional.SM_M3_CellFunctional'"));
	CellClosedMesh = CellClosedMesh_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellHoleMesh_RESOURCE(TEXT("StaticMesh'/M3/SM_M3_CellFunctional.SM_M3_CellFunctional'"));
	CellHoleMesh = CellHoleMesh_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CellRandomMesh_RESOURCE(TEXT("StaticMesh'/M3/SM_M3_CellFunctional.SM_M3_CellFunctional'"));
	CellRandomMesh = CellRandomMesh_RESOURCE.Get();

	static ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> CellClosedMaterial_RESOURCE(TEXT("MaterialInstanceConstant'/M3/M3_MI_RED.M3_MI_RED'"));
	CellClosedMaterial = CellClosedMaterial_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UMaterial> CellHoleMaterial_RESOURCE(TEXT("Material'/M3/M3_M_HOLE_ALPHA.M3_M_HOLE_ALPHA'"));
	CellHoleMaterial = CellHoleMaterial_RESOURCE.Get();
	static ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> CellRandomMaterial_RESOURCE(TEXT("MaterialInstanceConstant'/M3/M3_MI_GRAY.M3_MI_GRAY'"));
	CellRandomMaterial = CellRandomMaterial_RESOURCE.Get();

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CellSchemeRootComponent"));;

	ElementMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellSchemeElementMeshComponent"));
	ElementMeshComponent->SetupAttachment(GetRootComponent());
	ElementMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	BlockerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellSchemeBlockerMeshComponent"));
	BlockerMeshComponent->SetupAttachment(GetRootComponent());
	BlockerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlockerMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 1.f));
}

void AM3CellScheme::BeginPlay() {
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	UStaticMeshComponent* MeshComponent = nullptr;
	TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (ActorComponent->GetName() == "CellSchemeElementMeshComponent") {
			ElementMeshComponent = MeshComponent;
		} else if (ActorComponent->GetName() == "CellSchemeBlockerMeshComponent") {
			BlockerMeshComponent = MeshComponent;
		}
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

void AM3CellScheme::RemoveAppointment(EM3CellAppointment AppointmentId) {
	for (int i = 0; i < Appointments.Num(); ++i) {
		const auto& Appointment = Appointments[i];
		if (Appointment->Appointment == AppointmentId) {
			Appointments.RemoveAt(i);
			if (AppointmentId == EM3CellAppointment::FUNCTIONAL ||
				AppointmentId == EM3CellAppointment::REGULARELEMENT ||
				AppointmentId == EM3CellAppointment::SUPERELEMENT) {
				ElementMeshComponent->SetStaticMesh(nullptr);
				ElementMeshComponent->SetMaterial(0, nullptr);
			}
			if (AppointmentId == EM3CellAppointment::BLOCKER) {
				BlockerMeshComponent->SetStaticMesh(nullptr);
				BlockerMeshComponent->SetMaterial(0, nullptr);
			}
			break;
		}
	}
}

void AM3CellScheme::RemoveAllAppointments() {
	ElementMeshComponent->SetStaticMesh(nullptr);
	ElementMeshComponent->SetMaterial(0, nullptr);
	BlockerMeshComponent->SetStaticMesh(nullptr);
	BlockerMeshComponent->SetMaterial(0, nullptr);
	Appointments.Empty();
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

				GEditor->SelectNone(true, true);

				UWorld* World = GEditor->GetEditorWorldContext().World();
				AM3App* M3App = nullptr;
				for (TActorIterator<AActor> It(World, AM3App::StaticClass()); It; ++It) {
					M3App = static_cast<AM3App*>(*It);
					break;
				}
				if (M3App) {
					const auto BoardAssetsBundle = static_cast<UM3BoardAssetsBundle*>(M3App->AssetsBundle);
					if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::REGULARELEMENT) {
						RemoveAppointment(EM3CellAppointment::REGULARELEMENT);
						RemoveAppointment(EM3CellAppointment::SUPERELEMENT);
						const auto FunctionalAppointment = GetAppointment(EM3CellAppointment::SUPERELEMENT);
						if (FunctionalAppointment &&
							(FunctionalAppointment->Id == EM3ElementId::CELL_CLOSED ||
							 FunctionalAppointment->Id == EM3ElementId::CELL_HOLE ||
							 FunctionalAppointment->Id == EM3ElementId::CELL_RANDOM)) {
							RemoveAppointment(EM3CellAppointment::FUNCTIONAL);
						}
					}
					if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::SUPERELEMENT) {
						RemoveAppointment(EM3CellAppointment::REGULARELEMENT);
						RemoveAppointment(EM3CellAppointment::SUPERELEMENT);
						const auto FunctionalAppointment = GetAppointment(EM3CellAppointment::SUPERELEMENT);
						if (FunctionalAppointment &&
							(FunctionalAppointment->Id == EM3ElementId::CELL_CLOSED ||
							 FunctionalAppointment->Id == EM3ElementId::CELL_HOLE ||
							 FunctionalAppointment->Id == EM3ElementId::CELL_RANDOM)) {
							RemoveAppointment(EM3CellAppointment::FUNCTIONAL);
						}
					}

					if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::FUNCTIONAL) {
						RemoveAppointment(EM3CellAppointment::FUNCTIONAL);
						if (AM3CellScheme::EdModeSelectedAppointmentScheme->Id == EM3ElementId::CELL_HOLE) {
							RemoveAppointment(EM3CellAppointment::REGULARELEMENT);
							RemoveAppointment(EM3CellAppointment::SUPERELEMENT);
							RemoveAppointment(EM3CellAppointment::BLOCKER);
						}
						if (AM3CellScheme::EdModeSelectedAppointmentScheme->Id == EM3ElementId::CELL_CLOSED) {
							RemoveAppointment(EM3CellAppointment::REGULARELEMENT);
							RemoveAppointment(EM3CellAppointment::SUPERELEMENT);
							RemoveAppointment(EM3CellAppointment::BLOCKER);
						}
						if (AM3CellScheme::EdModeSelectedAppointmentScheme->Id == EM3ElementId::CELL_RANDOM) {
							RemoveAppointment(EM3CellAppointment::REGULARELEMENT);
							RemoveAppointment(EM3CellAppointment::SUPERELEMENT);
						}
					}

					if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::BLOCKER) {
						RemoveAppointment(EM3CellAppointment::BLOCKER);
						const auto RegularElementAppointment = GetAppointment(EM3CellAppointment::REGULARELEMENT);
						const auto SuperElementAppointment = GetAppointment(EM3CellAppointment::SUPERELEMENT);
						if (!RegularElementAppointment && !SuperElementAppointment) {
							return;
						}
						const auto FunctionalAppointment = GetAppointment(EM3CellAppointment::SUPERELEMENT);
						if (FunctionalAppointment &&
							(FunctionalAppointment->Id == EM3ElementId::CELL_CLOSED ||
							 FunctionalAppointment->Id == EM3ElementId::CELL_HOLE)) {
							return;
						}
						if (AM3CellScheme::EdModeSelectedAppointmentScheme->Id == EM3ElementId::BLOCKER_NONE) {
							return;
						}
					}
					
					AddAppointment(AM3CellScheme::EdModeSelectedAppointmentScheme);
					
					ensure(ElementMeshComponent != nullptr);
					ensure(BlockerMeshComponent != nullptr);

					if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::REGULARELEMENT) {
						switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
						case EM3ElementId::ELEMENT_RED:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_RED.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_RED.Material);
							break;
						case EM3ElementId::ELEMENT_GREEN:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_GREEN.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_GREEN.Material);
							break;
						case EM3ElementId::ELEMENT_BLUE:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_BLUE.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_BLUE.Material);
							break;
						case EM3ElementId::ELEMENT_YELLOW:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_YELLOW.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_YELLOW.Material);
							break;
						case EM3ElementId::ELEMENT_ORANGE:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_ORANGE.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_ORANGE.Material);
							break;
						case EM3ElementId::ELEMENT_PURPLE:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->Element_PURPLE.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->Element_PURPLE.Material);
							break;
						default:
							break;
						}
					} else if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::FUNCTIONAL) {
						switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
						case EM3ElementId::CELL_CLOSED:
							ElementMeshComponent->SetStaticMesh(CellClosedMesh);
							ElementMeshComponent->SetMaterial(0, CellClosedMaterial);
							break;
						case EM3ElementId::CELL_HOLE:
							ElementMeshComponent->SetStaticMesh(CellHoleMesh);
							ElementMeshComponent->SetMaterial(0, CellHoleMaterial);
							break;
						case EM3ElementId::CELL_RANDOM:
							ElementMeshComponent->SetStaticMesh(CellRandomMesh);
							ElementMeshComponent->SetMaterial(0, CellRandomMaterial);
							break;
						default:
							break;
						}
					} else if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::SUPERELEMENT) {
						switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
						case EM3ElementId::SUPERELEMENT_MATCH4:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH4.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH4.Material);
							break;
						case EM3ElementId::SUPERELEMENT_MATCH5:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH5.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH5.Material);
							break;
						case EM3ElementId::SUPERELEMENT_MATCH6:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH6.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH6.Material);
							break;
						case EM3ElementId::SUPERELEMENT_MATCH7:
							ElementMeshComponent->SetStaticMesh(BoardAssetsBundle->SuperElement_MATCH7.Mesh);
							ElementMeshComponent->SetMaterial(0, BoardAssetsBundle->SuperElement_MATCH7.Material);
							break;
						default:
							break;
						}
					}
					else if (AM3CellScheme::EdModeSelectedAppointmentScheme->Appointment == EM3CellAppointment::BLOCKER) {
						switch (AM3CellScheme::EdModeSelectedAppointmentScheme->Id) {
						case EM3ElementId::BLOCKER_NONE:
							BlockerMeshComponent->SetStaticMesh(nullptr);
							BlockerMeshComponent->SetMaterial(0, nullptr);
							break;
						case EM3ElementId::BLOCKER_BOX1X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box1X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box1X.Material);
							break;
						case EM3ElementId::BLOCKER_BOX2X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box2X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box2X.Material);
							break;
						case EM3ElementId::BLOCKER_BOX3X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Box3X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Box3X.Material);
							break;
						case EM3ElementId::BLOCKER_ICE1X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Ice1X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Ice1X.Material);
							break;
						case EM3ElementId::BLOCKER_ICE2X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Ice2X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Ice2X.Material);
							break;
						case EM3ElementId::BLOCKER_WIRE1X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Wire1X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Wire1X.Material);
							break;
						case EM3ElementId::BLOCKER_WIRE2X:
							BlockerMeshComponent->SetStaticMesh(BoardAssetsBundle->Blocker_Wire2X.Mesh);
							BlockerMeshComponent->SetMaterial(0, BoardAssetsBundle->Blocker_Wire2X.Material);
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

#endif

AM3GoalScheme::AM3GoalScheme() {
	PrimaryActorTick.bCanEverTick = true;
	auto MeshComponent = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (MeshComponent) {
		Cast<UStaticMeshComponent>(MeshComponent)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

AM3BoardScheme::AM3BoardScheme() {
	PrimaryActorTick.bCanEverTick = true;
	auto MeshComponent = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (MeshComponent) {
		Cast<UStaticMeshComponent>(MeshComponent)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

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