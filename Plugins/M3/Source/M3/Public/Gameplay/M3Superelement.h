// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3Superelement.generated.h"

FORWARD_DECL_STRONG(M3SuperelementModel)
FORWARD_DECL_STRONG(M3SuperelementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3Superelement : public ACharacter {
	GENERATED_BODY()

public:

	M3SuperelementModel_SharedPtr Model = nullptr;
	M3SuperelementView_SharedPtr View = nullptr;

	AM3Superelement();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnLoad(UM3AssetsBundle* _Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel);
	void OnBindViewDelegate();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	void Dispose();
};
