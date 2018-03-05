// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3ViewDelegate.h"
#include "M3ElementView.generated.h"

DECLARE_DYNAMIC_DELEGATE(FElementViewAnimationDelegate);
DECLARE_DELEGATE(FElementViewAnimationCallback);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Accessors))
class HEXMAP_API UM3ElementViewAccessor : public UObject {
private:

	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Delegates")
	void OnSwapEnded();

public:

	UM3ElementViewAccessor();

	UPROPERTY(Category = "Accessors", EditAnywhere, BlueprintReadWrite)
	AActor* View = nullptr;

	UPROPERTY(Category = "Accessors", EditAnywhere, BlueprintReadWrite)
	FElementViewAnimationDelegate OnSwapEndedDelegate;
	FElementViewAnimationCallback OnSwapEndedCallback;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class HEXMAP_API UM3ElementViewDelegate : public UM3ViewDelegate_INTERFACE
{
private:

	GENERATED_BODY()

public:

	UM3ElementViewDelegate();
	~UM3ElementViewDelegate();

	int ViewSTTI() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Delegates")
	void OnSwap(UM3ElementViewAccessor* Accessor);
};

class HEXMAP_API M3ElementView : public M3View
{
private:

	UM3ElementViewAccessor* Accessor = nullptr;

public:

	M3ElementView(AActor* Superview);

	CTTI_CLASS_GUID(M3ElementView, M3View_INTERFACE::GuidsContainer)

	void Load(AM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
