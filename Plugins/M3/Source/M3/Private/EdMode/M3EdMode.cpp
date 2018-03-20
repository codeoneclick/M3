// Copyright serhii serhiiv 2017. All rights reserved.

#include "M3EdMode.h"

#if WITH_EDITOR

#include "Engine.h"
#include "M3EdModeToolkit.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Toolkits/ToolkitManager.h"
#include "M3Scheme.h"
#include "M3EdModeProperties.h"

FEditorModeID FM3EdMode::EM_M3(TEXT("EM_M3"));

FM3EdMode::FM3EdMode() {
	EdModeProps_BoardCreate = NewObject<UM3EdModeProps_BoardCreate>(GetTransientPackage(), TEXT("EdModelProps_BoardCreate"), RF_Transactional | RF_MarkAsRootSet);
	EdModeProps_BoardScheme = NewObject<UM3EdModeProps_BoardScheme>(GetTransientPackage(), TEXT("EdModelProps_BoardScheme"), RF_Transactional | RF_MarkAsRootSet);
	EdModeProps_BoardReskin = NewObject<UM3EdModeProps_BoardReskin>(GetTransientPackage(), TEXT("EdModelProps_BoardReskin"), RF_Transactional | RF_MarkAsRootSet);
}

void FM3EdMode::Enter()
{
	FEdMode::Enter();
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FM3EdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FM3EdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	FEdMode::Exit();
}

void FM3EdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime) {
	FEdMode::Tick(ViewportClient, DeltaTime);
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorTick(DeltaTime);
	}
}

bool FM3EdMode::StartTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport) {
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorMousePressed();
	}
	return FEdMode::StartTracking(ViewportClient, InViewport);
}

bool FM3EdMode::EndTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport) {
	for (TActorIterator<AM3CellScheme> It(ViewportClient->GetWorld()); It; ++It) {
		AM3CellScheme* Actor = *It;
		Actor->OnEditorMouseReleased();
	}
	return FEdMode::EndTracking(ViewportClient, InViewport);
}

bool FM3EdMode::UsesToolkits() const
{
	return true;
}

#endif
