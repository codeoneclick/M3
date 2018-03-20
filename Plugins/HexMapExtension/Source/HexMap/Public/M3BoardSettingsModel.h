// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"
#include "M3Scheme.h"

class M3BoardSettingsEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3BoardSettingsEntity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3BoardSettingsEntity, Turns, int, 0)
	PROP_STRONG(public, M3BoardSettingsEntity, ElementSize, FVector2D, FVector2D(-1, -1))
	PROP_STRONG(public, M3BoardSettingsEntity, ElementIds, std::shared_ptr<std::vector<EM3ElementId>>, std::make_shared<std::vector<EM3ElementId>>())
};

class HEXMAP_API M3BoardSettingsModel : public M3Model<M3BoardSettingsEntity>
{
public:

	CTTI_CLASS_GUID(M3BoardSettingsModel, M3Model_INTERFACE::GuidsContainer)

	M3BoardSettingsModel();
	~M3BoardSettingsModel();

	void Init() override;

	void Serialize() override;
	void Deserialize(AM3Scheme_INTERFACE* Scheme) override;

	PROP_DECL_R(Turns, int)
	PROP_DECL_R(ElementSize, FVector2D)
	PROP_DECL_R(ElementIds, std::shared_ptr<std::vector<EM3ElementId>>)
};
