// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3SuperElement.generated.h"

FORWARD_DECL_STRONG(M3SuperElementModel)
FORWARD_DECL_STRONG(M3SuperElementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3SuperElement : public ACharacter {
	GENERATED_BODY()

private:

	M3SuperElementModel_SharedPtr Model = nullptr;
	M3SuperElementView_SharedPtr View = nullptr;

public:

	AM3SuperElement();

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
