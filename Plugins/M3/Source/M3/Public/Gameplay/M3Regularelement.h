// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3RegularElement.generated.h"

FORWARD_DECL_STRONG(M3RegularElementModel)
FORWARD_DECL_STRONG(M3RegularElementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3RegularElement : public ACharacter {

	GENERATED_BODY()

public:

	AM3RegularElement();
	~AM3RegularElement();

protected:

	M3RegularElementModel_SharedPtr Model = nullptr;
	M3RegularElementView_SharedPtr View = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void OnLoad(UM3AssetsBundle* Bundle);
	virtual void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);
	virtual void OnBindViewDelegate();
	virtual void OnBindViewAccessor();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	void Dispose();
};
