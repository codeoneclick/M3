// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3ViewDelegate.h"
#include "M3ElementView.generated.h"

DECLARE_DYNAMIC_DELEGATE(FElementViewAnimationDelegate);
DECLARE_DELEGATE(FElementViewAnimationCallback);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Accessors))
class HEXMAP_API UM3ElementViewAccessor : public UObject {
private:

	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "M3Delegates")
	void OnSwapEnded();
	
	UFUNCTION(BlueprintCallable, Category = "M3Delegates")
	void OnMatchEnded();

	UFUNCTION(BlueprintCallable, Category = "M3Delegates")
	void OnDropEnded();

	UFUNCTION(BlueprintCallable, Category = "M3Delegates")
	void OnSpawnEnded();

public:

	UM3ElementViewAccessor();

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	AActor* View = nullptr;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	int CurrentCol = -1;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	int CurrentRow = -1;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	int OppositeCol = -1;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	int OppositeRow = -1;

	UPROPERTY(Category = "Accessors", EditAnywhere, BlueprintReadWrite)
	FVector2D ElementSize = FVector2D(-1, -1);

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	bool IsPosibleToSwap = false;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	FElementViewAnimationDelegate OnSwapEndedDelegate;
	FElementViewAnimationCallback OnSwapEndedCallback;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	FElementViewAnimationDelegate OnMatchEndedDelegate;
	FElementViewAnimationCallback OnMatchEndedCallback;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	FElementViewAnimationDelegate OnDropEndedDelegate;
	FElementViewAnimationCallback OnDropEndedCallback;

	UPROPERTY(Category = "M3Accessors", EditAnywhere, BlueprintReadWrite)
	FElementViewAnimationDelegate OnSpawnEndedDelegate;
	FElementViewAnimationCallback OnSpawnEndedCallback;
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

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnSwap(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnMatch(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnDrop(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintImplementableEvent, Category = "M3Delegates")
	void OnSpawn(UM3ElementViewAccessor* Accessor);
};

class HEXMAP_API M3ElementView : public M3View
{
private:

	UM3ElementViewAccessor* Accessor = nullptr;

	void SetElementVisual(int ElementId);

public:

	M3ElementView(AActor* _Superview);
	~M3ElementView();

	CTTI_CLASS_GUID(M3ElementView, M3View_INTERFACE::GuidsContainer)

	void Load(UM3AssetsBundle* Bundle);
	void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
