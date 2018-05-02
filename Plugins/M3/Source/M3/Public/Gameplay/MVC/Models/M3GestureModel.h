// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

enum EM3Gesture {
	UNKNOWN = 0,
	TAP,
	DOUBLE_TAP,
	PAN_START,
	PAN,
	PAN_END,
};

struct M3Gesture {
public:

	EM3Gesture Gesture;
	FVector Location;
};

class M3GestureEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3GestureEntity)

	PROP_STRONG(public, M3GestureEntity, Gestures, std::shared_ptr<std::list<M3Gesture>>, std::make_shared<std::list<M3Gesture>>())
	PROP_STRONG(public, M3GestureEntity, IsPanned, bool, false)
	PROP_STRONG(public, M3GestureEntity, IsInterrupted, bool, false)
};

class M3_API M3GestureModel : public M3Model<M3GestureEntity>
{
public:

	M3GestureModel();
	~M3GestureModel();

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Init();

	PROP_DECL_R(IsPanned, bool)
	PROP_DECL_W(IsPanned, bool)

	PROP_DECL_R(IsInterrupted, bool)
	PROP_DECL_W(IsInterrupted, bool)

	int GetGesturesNum() const;

	bool IsLastGestureIs(EM3Gesture Gesture) const;

	void PushGesture(EM3Gesture Gesture, const FVector& Location);
	M3Gesture PopGesture();
};
