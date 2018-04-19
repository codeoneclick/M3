// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

FORWARD_DECL_STRONG(M3ElementModel)

enum EM3ElementState {
	IDLE = 0,
	SPAWNING,
	SWAPPING,
	DROPPING,
	MATCHING,
	REMOVING,
};

enum EM3ElementColor {
	NONE = 0,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	PURPLE
};

class M3_API M3ElementModelColorComponent_INTERFACE {
public:
	virtual EM3ElementColor GetColor() = 0;
};

class M3_API M3ElementModelBlockerComponent_INTERFACE {
public:
	virtual bool CanDrop() = 0;
	virtual bool CanSwap() = 0;
	virtual bool CanMatch() = 0;
	virtual bool CanRemove() = 0;
};

class M3_API M3ElementModelColorComponent : public M3ModelComponent_INTERFACE {
private:

	std::shared_ptr<M3ElementModelColorComponent_INTERFACE> Owner = nullptr;

public:

	CTTI_CLASS_GUID(M3ElementModelColorComponent, M3ModelComponent_INTERFACE::GuidsContainer)

	M3ElementModelColorComponent(const std::shared_ptr<M3ElementModelColorComponent_INTERFACE>& _Owner);
	EM3ElementColor GetColor();
};

class M3_API M3ElementModelBlockerComponent : public M3ModelComponent_INTERFACE {
private:

	std::shared_ptr<M3ElementModelBlockerComponent_INTERFACE> Owner = nullptr;

public:

	CTTI_CLASS_GUID(M3ElementModelBlockerComponent, M3ModelComponent_INTERFACE::GuidsContainer)

	M3ElementModelBlockerComponent(const std::shared_ptr<M3ElementModelBlockerComponent_INTERFACE>& _Owner);
	bool CanDrop() const;
	bool CanSwap() const;
	bool CanMatch() const;
	bool CanRemove() const;
};

class M3_API M3ElementEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3ElementEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3ElementEntity, State, EM3ElementState, EM3ElementState::IDLE)
	PROP_STRONG(public, M3ElementEntity, Timestamp, int64, 0)
};

class M3_API M3ElementModel : public M3Model<M3ElementEntity> {
public:

	M3ElementModel();
	~M3ElementModel();

	PROP_DECL_R(Timestamp, int64)

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void Reset() override;

	EM3ElementColor GetColor();

	void SetState(EM3ElementState State);
	EM3ElementState GetState() const;

	bool IsInState(EM3ElementState State) const;

	bool IsInIdle() const;
	bool CanMatch() const;
	bool CanDrop() const;
	bool CanSwap() const;
	bool CanRemove() const;

	bool IsDropBlocked() const;
	bool IsMatchBlocked() const;

	bool IsRegular() const;
	bool IsSuper() const;

	static bool IsNeighbours(const M3ElementModel_SharedPtr& CurrentElementModel, const M3ElementModel_SharedPtr& NeighbourElementModel);
};
