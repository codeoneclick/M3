// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "Components/ActorComponent.h"
#include "M3InteractionComponent.generated.h"

FORWARD_DECL_STRONG(M3ElementModel)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class M3_API UM3InteractionComponent : public UActorComponent {

	GENERATED_BODY()

public:	

	UM3InteractionComponent();

protected:

	M3ElementModel_SharedPtr ElementModel = nullptr;

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadWrite)
	bool isDoubleTapEnabled = false;

	void SetElementModel(const M3ElementModel_SharedPtr& _ElementModel);
	M3ElementModel_SharedPtr GetElementModel() const;
};
