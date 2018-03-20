// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Entity.h"
#include "M3Model.h"

FORWARD_DECL_STRONG(M3ElementModel)
FORWARD_DECL_STRONG(M3Chain)
FORWARD_DECL_STRONG(M3BoardModel)

enum EM3Chain {
	UNKNOWN_CHAIN = 0,
	HORIZONTAL,
	VERTICAL,
};

class M3Chain {
public:

	EM3Chain Chain = EM3Chain::UNKNOWN_CHAIN;
	std::vector<M3ElementModel_SharedPtr> Elements;
};


class M3_API M3ChainEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3ChainEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3ChainEntity, HorizontalChains, std::shared_ptr<std::vector<M3Chain_SharedPtr>>, std::make_shared<std::vector<M3Chain_SharedPtr>>())
	PROP_STRONG(public, M3ChainEntity, VerticalChains, std::shared_ptr<std::vector<M3Chain_SharedPtr>>, std::make_shared<std::vector<M3Chain_SharedPtr>>())
};


class M3_API M3ChainModel : public M3Model<M3ChainEntity>
{
protected: 

	void PushChain(const M3Chain_SharedPtr& Chain);

public:

	M3ChainModel();
	~M3ChainModel();

	CTTI_CLASS_GUID(M3ChainModel, M3Model<M3ChainEntity>::GuidsContainer)

	void Init();

	void Serialize();
	void Deserialize(AM3Scheme_INTERFACE* Scheme);

	void DetectHorizontalMatches(const M3BoardModel_SharedPtr& BoardModel);
	void DetectVerticalMatches(const M3BoardModel_SharedPtr& BoardModel);

	bool IsChainsExist() const;
	M3Chain_SharedPtr PopChain();
};
