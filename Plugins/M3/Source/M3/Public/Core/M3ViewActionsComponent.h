// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "Components/ActorComponent.h"
#include "M3Utilities.h"
#include "M3ViewActionsComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FM3ActionCallback);
DECLARE_DELEGATE(FM3ActionNativeCallback);

FORWARD_DECL_STRONG(UM3Action)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions), meta = (BlueprintSpawnableComponent))
class M3_API UM3ViewActionsComponent : public UActorComponent {

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UM3ViewActionsComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = "M3Actions", EditAnywhere, BlueprintReadWrite)
	TArray<UM3Action*> Actions;

	UFUNCTION(BlueprintCallable, Category = "M3Actions")
	void RunAction(UM3Action* Action);

	UFUNCTION(BlueprintCallable, Category = "M3Actions")
	UM3Action* MoveToAction(float Duration, const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "M3Actions")
	UM3Action* ScaleToAction(float Duration, const FVector& Scale);

	UFUNCTION(BlueprintCallable, Category = "M3Actions")
	UM3Action* CallbackAction(float Duration, FM3ActionCallback Callback);
	UM3Action* CallbackAction(float Duration, FM3ActionNativeCallback Callback);
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions))
class M3_API UM3Action : public UObject {

	GENERATED_BODY()

protected:

	AActor* Actor = nullptr;

	float Duration = 0;
	float CurrentDuration = 0;

	bool bIsDone = false;

	void Setup(AActor* _Actor, float _Duration);

public:

	UM3Action();
	virtual ~UM3Action() = default;

	virtual void OnUpdate(float DeltaTime);
	virtual bool IsDone() const;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions))
class M3_API UM3Sequence : public UM3Action {

	GENERATED_BODY()

protected:

public:

	UM3Sequence();
	virtual ~UM3Sequence() = default;

	virtual void Setup(AActor* _Actor);

	UPROPERTY(Category = "M3Actions", EditAnywhere, BlueprintReadWrite)
	TArray<UM3Action*> Actions;

	virtual void OnUpdate(float DeltaTime);
	virtual bool IsDone() const;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions))
class M3_API UM3MoveToAction : public UM3Action {

	GENERATED_BODY()

protected:

	FVector OldLocation;
	FVector MoveToLocation;

public:

	UM3MoveToAction();
	virtual ~UM3MoveToAction() = default;

	void Setup(AActor* _Actor, float _Duration, const FVector& Location);

	virtual void OnUpdate(float DeltaTime);
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions))
class M3_API UM3ScaleToAction : public UM3Action {

	GENERATED_BODY()

protected:

	FVector OldScale;
	FVector ScaleTo;

public:

	UM3ScaleToAction();
	virtual ~UM3ScaleToAction() = default;

	void Setup(AActor* _Actor, float _Duration, const FVector& Scale);

	virtual void OnUpdate(float DeltaTime);
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (M3Actions))
class M3_API UM3CallbackAction : public UM3Action {

	GENERATED_BODY()

protected:

	FM3ActionNativeCallback NativeCallback;

public:

	UM3CallbackAction();
	virtual ~UM3CallbackAction() = default;

	UPROPERTY(Category = "M3Actions", EditAnywhere, BlueprintReadWrite)
	FM3ActionCallback Callback;

	void Setup(AActor* _Actor, float _Duration, FM3ActionCallback _Callback);
	void Setup(AActor* _Actor, float _Duration, FM3ActionNativeCallback _Callback);

	virtual void OnUpdate(float DeltaTime);
};


