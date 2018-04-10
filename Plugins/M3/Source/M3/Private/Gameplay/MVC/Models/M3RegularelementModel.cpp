// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3RegularElementModel.h"

M3RegularElementModel::M3RegularElementModel() {

}

M3RegularElementModel::~M3RegularElementModel() {
}

void M3RegularElementModel::Init() {
	SUBSCRIBE_PROP(M3RegularElementEntity, Id)
	std::shared_ptr<M3RegularElementModel> Instance = std::static_pointer_cast<M3RegularElementModel>(shared_from_this());
	AddComponent(std::make_shared<M3ElementModelColorComponent>(std::static_pointer_cast<M3ElementModelColorComponent_INTERFACE>(Instance)));
}

void M3RegularElementModel::Serialize() {

}

void M3RegularElementModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

EM3ElementColor M3RegularElementModel::GetColor() {
	EM3ElementColor Result = EM3ElementColor::NONE;
	switch (GetId())
	{
	case EM3ElementId::ELEMENT_RED:
		Result = EM3ElementColor::RED;
		break;
	case EM3ElementId::ELEMENT_GREEN:
		Result = EM3ElementColor::GREEN;
		break;
	case EM3ElementId::ELEMENT_BLUE:
		Result = EM3ElementColor::BLUE;
		break;
	case EM3ElementId::ELEMENT_YELLOW:
		Result = EM3ElementColor::YELLOW;
		break;
	case EM3ElementId::ELEMENT_ORANGE:
		Result = EM3ElementColor::ORANGE;
		break;
	case EM3ElementId::ELEMENT_PURPLE:
		Result = EM3ElementColor::PURPLE;
		break;
	default:
		break;
	}

	return Result;
}

void M3RegularElementModel::Reset() {
	M3Model::Reset();

	Entity->Get()->Id->Set(EM3ElementId::UNKNOWN);
}
