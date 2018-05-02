// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

enum EM3AccumulationAction {
	ACCUMULATION_ACTION_UNKNOWN = 0,
	ON_ELEMENT_SWAP_ENDED,
	ON_ELEMENT_MATCH_ENDED,
	ON_ELEMENT_DROP_ENDED,
	ON_ELEMENT_SPAWN_ENDED,
	ON_ELEMENT_SHUFFLE_ENDED,
};

struct M3AccumulationAction {
public:

	EM3AccumulationAction Action = EM3AccumulationAction::ACCUMULATION_ACTION_UNKNOWN;
	int Revision = 0;
};

class M3BoardActionsAccumulationEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardActionsAccumulationEntity)

	PROP_STRONG(public, M3BoardActionsAccumulationEntity, Actions, std::shared_ptr<std::list<M3AccumulationAction>>, std::make_shared<std::list<M3AccumulationAction>>())
	PROP_STRONG(public, M3BoardActionsAccumulationEntity, Revision, int, 0)
};

class M3_API M3BoardActionsAccumulationModel : public M3Model<M3BoardActionsAccumulationEntity>
{
public:

	M3BoardActionsAccumulationModel();
	~M3BoardActionsAccumulationModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	void PushAction(EM3AccumulationAction Action);
	bool HasActions() const;
};
