// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3View.h"
#include "M3ViewDelegate.h"
#include "M3ElementView.generated.h"

DECLARE_DYNAMIC_DELEGATE(FElementViewAnimationDelegate);
DECLARE_DELEGATE(FElementViewAnimationCallback);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Accessors))
class M3_API UM3ElementViewAccessor : public UObject {
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

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	AActor* View = nullptr;

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	int CurrentCol = -1;

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	int CurrentRow = -1;

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	int OppositeCol = -1;

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	int OppositeRow = -1;

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
	FVector2D ElementSize = FVector2D(-1, -1);

	UPROPERTY(Category = "M3Accessors", VisibleAnywhere, BlueprintReadOnly)
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

	UFUNCTION(BlueprintCallable, Category = "M3Accessors")
	FVector GetCurrentLocation();

	UFUNCTION(BlueprintCallable, Category = "M3Accessors")
	FVector GetOppositeLocation();
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Delegates))
class M3_API UM3ElementViewDelegate : public UM3ViewDelegate_INTERFACE
{
private:

	GENERATED_BODY()

public:

	UM3ElementViewDelegate();
	~UM3ElementViewDelegate();

	UFUNCTION(BlueprintNativeEvent, Category = "M3Delegates")
	void OnSwap(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintNativeEvent, Category = "M3Delegates")
	void OnMatch(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintNativeEvent, Category = "M3Delegates")
	void OnDrop(UM3ElementViewAccessor* Accessor);

	UFUNCTION(BlueprintNativeEvent, Category = "M3Delegates")
	void OnSpawn(UM3ElementViewAccessor* Accessor);
};

class M3_API M3ElementView : public M3View
{
protected:

	UM3ElementViewAccessor* Accessor = nullptr;

public:

	CTTI_CLASS_GUID(M3ElementView, M3View_INTERFACE::GuidsContainer)

	M3ElementView(AActor* _Superview);
	~M3ElementView();

	virtual void Load(UM3AssetsBundle* Bundle);
	virtual void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);
};
