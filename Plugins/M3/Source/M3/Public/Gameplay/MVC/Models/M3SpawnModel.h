// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"
#include "M3Scheme.h"

FORWARD_DECL_STRONG(M3CellModel)

class M3SuperElementSpawner {
protected:

	M3CellModel_SharedPtr Cell = nullptr;
	int Length = 0;

public:
	M3SuperElementSpawner(const M3CellModel_SharedPtr& _Cell, int _Length);

	M3CellModel_SharedPtr GetCell() const;
	int GetLength() const;
};

FORWARD_DECL_STRONG(M3SuperElementSpawner)

class M3SpawnEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3SpawnEntity)

	PROP_STRONG(public, M3SpawnEntity, SuperElementSpawners, std::shared_ptr<std::list<M3SuperElementSpawner_SharedPtr>>, std::make_shared<std::list<M3SuperElementSpawner_SharedPtr>>())
};

class M3_API M3SpawnModel : public M3Model<M3SpawnEntity> {
public:

	M3SpawnModel();
	~M3SpawnModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	bool IsSuperElementSpawnersExist() const;
	void PushSuperElementSpawner(const M3CellModel_SharedPtr& Cell, int Length);
	M3SuperElementSpawner_SharedPtr PopSuperElementSpawner();
};
