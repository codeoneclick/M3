// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

enum EM3ElementState {
	IDLE = 0,
	SPAWNING,
	SWAPPING,
	DROPPING,
	MATCHING,
	REMOVING,
};

class M3ElementEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3ElementEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3ElementEntity, ElementId, int, -1)
	PROP_STRONG(public, M3ElementEntity, State, EM3ElementState, EM3ElementState::IDLE)
};

class HEXMAP_API M3ElementModel : public M3Model<M3ElementEntity>
{
public:

	M3ElementModel();
	~M3ElementModel();

	CTTI_CLASS_GUID(M3ElementModel, M3Model_INTERFACE::GuidsContainer)

	void Init();

	void Serialize();
	void Deserialize(UM3Scheme_INTERFACE* Scheme);

	int GetElementId() const;

	void SetState(EM3ElementState State);
	EM3ElementState GetState() const;
	bool IsInState(EM3ElementState State) const;
};
