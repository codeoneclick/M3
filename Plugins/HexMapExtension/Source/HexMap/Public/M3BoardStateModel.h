// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"

FORWARD_DECL_STRONG(M3ElementModel)

class M3BoardStateEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardStateEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3BoardStateEntity, UnusedElements, std::shared_ptr<std::vector<M3ElementModel_SharedPtr>>, std::make_shared<std::vector<M3ElementModel_SharedPtr>>())
	PROP_STRONG(public, M3BoardStateEntity, IsHolesExist, bool, true)
};

class HEXMAP_API M3BoardStateModel : public M3Model<M3BoardStateEntity>
{

public:

	M3BoardStateModel();
	~M3BoardStateModel();

	void Init();

	void Serialize();
	void Deserialize(UM3Scheme_INTERFACE* Scheme);

	PROP_DECL_R(IsHolesExist, bool)
	PROP_DECL_W(IsHolesExist, bool)

	void PushUnusedElement(const M3ElementModel_SharedPtr& Element);
	M3ElementModel_SharedPtr PopUnusedElement();
};
