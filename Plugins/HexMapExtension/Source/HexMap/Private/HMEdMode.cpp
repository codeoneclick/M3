// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMEdMode.h"

#if WITH_EDITOR

#include "Engine.h"
#include "HMEdModeToolkit.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Toolkits/ToolkitManager.h"
#include "HMTile.h"
#include "HMGrid.h"
#include "M3Scheme.h"
#include "HMEdModeProperties.h"

FEditorModeID FHMEdMode::EM_HexMap(TEXT("EM_HexMap"));

FHMEdMode::FHMEdMode() {
	EdModeProps_BoardCreate = NewObject<UM3EdModeProps_BoardCreate>(GetTransientPackage(), TEXT("EdModelProps_BoardCreate"), RF_Transactional | RF_MarkAsRootSet);
	EdModeProps_BoardScheme = NewObject<UM3EdModeProps_BoardScheme>(GetTransientPackage(), TEXT("EdModelProps_BoardScheme"), RF_Transactional | RF_MarkAsRootSet);
	EdModeProps_BoardReskin = NewObject<UM3EdModeProps_BoardReskin>(GetTransientPackage(), TEXT("EdModelProps_BoardReskin"), RF_Transactional | RF_MarkAsRootSet);

	EdModePropertiesSetTileSize = NewObject<UHMEdModePropertiesSetTileSize>(GetTransientPackage(), TEXT("EdModePropertiesSetTileSize"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddCircle = NewObject<UHMEdModePropertiesAddCircle>(GetTransientPackage(), TEXT("EdModePropertiesAddCircle"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddRectangle = NewObject<UHMEdModePropertiesAddRectangle>(GetTransientPackage(), TEXT("EdModePropertiesAddRectangle"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddTile = NewObject<UHMEdModePropertiesAddTile>(GetTransientPackage(), TEXT("EdModePropertiesAddTile"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesRandomizeTiles = NewObject<UHMEdModePropertiesRandomizeTiles>(GetTransientPackage(), TEXT("EdModePropertiesRandomizeTiles"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesTileBatchApplier = NewObject<UHMEdModePropertiesTileBatchApplier>(GetTransientPackage(), TEXT("EdModePropertiesTileBatchApplier"), RF_Transactional | RF_MarkAsRootSet);
}

void FHMEdMode::Enter()
{
	FEdMode::Enter();
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FHMEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FHMEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	FEdMode::Exit();
}

void FHMEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime) {
	FEdMode::Tick(ViewportClient, DeltaTime);
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorTick(DeltaTime);
	}
}

bool FHMEdMode::StartTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport) {
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorMousePressed();
	}
	return FEdMode::StartTracking(ViewportClient, InViewport);
}

bool FHMEdMode::EndTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport) {
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorMouseReleased();
	}
	return FEdMode::EndTracking(ViewportClient, InViewport);
}

bool FHMEdMode::UsesToolkits() const
{
	return true;
}

#endif
