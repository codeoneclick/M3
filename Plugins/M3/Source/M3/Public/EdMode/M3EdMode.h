// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#if WITH_EDITOR

#include "EdMode.h"

class FM3EdMode : public FEdMode
{
protected:

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	virtual bool UsesToolkits() const override;

public:

	FM3EdMode();
	virtual ~FM3EdMode() = default;

	virtual void Enter() override;
	virtual void Exit() override;

	class UM3EdModeProps_BoardCreate* EdModeProps_BoardCreate;
	class UM3EdModeProps_BoardScheme* EdModeProps_BoardScheme;
	class UM3EdModeProps_BoardReskin* EdModeProps_BoardReskin;

	static FEditorModeID EM_M3;
};

#endif
