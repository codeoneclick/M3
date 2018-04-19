// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3Blocker.generated.h"

FORWARD_DECL_STRONG(M3BlockerModel)
FORWARD_DECL_STRONG(M3BlockerView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)

UCLASS()
class M3_API AM3Blocker : public ACharacter {
	GENERATED_BODY()

private:

	M3BlockerModel_SharedPtr Model = nullptr;
	M3BlockerView_SharedPtr View = nullptr;

public:

	
	AM3Blocker();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void OnLoad(UM3AssetsBundle* _Bundle);
	virtual void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel);
	virtual void OnBindViewDelegate();
	virtual void OnBindViewAccessor();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	void Dispose();
};
