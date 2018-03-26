// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Element.h"
#include "M3ElementModel.h"
#include "M3ElementView.h"
#include "M3AssetsBundle.h"
#include "Components/StaticMeshComponent.h"
#include "M3ViewActionsComponent.h"

#if defined(__DESTRUCTIBLE_MESH__)

#include "DestructibleComponent.h"
#include "DestructibleMesh.h"

#endif

AM3Element::AM3Element() {
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ElementRootComponent"));;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UM3ViewActionsComponent* ActionsComponent = CreateDefaultSubobject<UM3ViewActionsComponent>(TEXT("ElementActionsComponent"));

#if defined(__DESTRUCTIBLE_MESH__)

	UDestructibleComponent *DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("ElementDestructibleComponent"));;
	DestructibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->SetSimulatePhysics(false);
	DestructibleComponent->SetEnableGravity(false);
	DestructibleComponent->WakeRigidBody(NAME_None);

#endif
}

void AM3Element::BeginPlay() {
	Super::BeginPlay();
}

void AM3Element::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AM3Element::OnLoad(UM3AssetsBundle* Bundle) {
	assert(false);
}

void AM3Element::OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model) {
	assert(false);

#if defined(__DESTRUCTIBLE_MESH__)

	UStaticMeshComponent* MeshComponent = nullptr;
	TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ActorComponent : MeshesComponents) {
		MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		break;
	}

	UDestructibleMesh* DestructibleMesh = NewObject<UDestructibleMesh>();
	DestructibleMesh->BuildFromStaticMesh((*MeshComponent->GetStaticMesh()));

	UDestructibleComponent* DestructionComponent = nullptr;
	TArray<UActorComponent*> DestructionComponents = GetComponentsByClass(UDestructibleComponent::StaticClass());
	for (UActorComponent* ActorComponent : DestructionComponents) {
		DestructionComponent = Cast<UDestructibleComponent>(ActorComponent);
		break;
	}
	DestructionComponent->SetDestructibleMesh(DestructibleMesh);

#endif
}

void AM3Element::OnBindViewDelegate() {
	if (Delegate_BP) {
		Delegate = NewObject<UM3ViewDelegate_INTERFACE>(this, Delegate_BP);
	} else {
		Delegate = NewObject<UM3ElementViewDelegate>();
	}
	ElementView->BindViewDelegate(Delegate);
}

M3View_INTERFACE_SharedPtr AM3Element::GetView() const {
	return ElementView;
}

M3Model_INTERFACE_SharedPtr AM3Element::GetModel() const {
	return ElementModel;
}

