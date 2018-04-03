// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M3Utilities.h"
#include "M3Regularelement.generated.h"

FORWARD_DECL_STRONG(M3RegularelementModel)
FORWARD_DECL_STRONG(M3RegularelementView)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Views))
class M3_API AM3Regularelement : public ACharacter {

	GENERATED_BODY()

public:

	AM3Regularelement();
	~AM3Regularelement();

protected:

	M3RegularelementModel_SharedPtr Model = nullptr;
	M3RegularelementView_SharedPtr View = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnLoad(UM3AssetsBundle* Bundle);
	void OnBindViewModel(const M3Model_INTERFACE_SharedPtr& Model);
	void OnBindViewDelegate();

	M3View_INTERFACE_SharedPtr GetView() const;
	M3Model_INTERFACE_SharedPtr GetModel() const;

	void Dispose();
};
