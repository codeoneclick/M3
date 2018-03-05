// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

enum EM3Gesture {
	UNKNOWN = 0,
	TAP,
	PAN,
};

struct M3Gesture {
public:

	EM3Gesture Gesture;
	FVector Location;
};

class M3GestureEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3GestureEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3GestureEntity, Gestures, std::shared_ptr<std::list<M3Gesture>>, std::make_shared<std::list<M3Gesture>>())
};



class HEXMAP_API M3GestureModel : public M3Model<M3GestureEntity>
{
public:

	M3GestureModel();
	~M3GestureModel();

	CTTI_CLASS_GUID(M3GestureModel, M3Model_INTERFACE::GuidsContainer)

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init();

	int GetGesturesNum() const;

	bool IsLastGestureIs(EM3Gesture Gesture) const;

	void PushGesture(EM3Gesture Gesture, const FVector& Location);
	M3Gesture PopGesture();
};