// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardStateModel.h"
#include "M3ElementModel.h"

M3BoardStateModel::M3BoardStateModel() {
}

M3BoardStateModel::~M3BoardStateModel() {
}

void M3BoardStateModel::Init() {

}

void M3BoardStateModel::Serialize() {

}

void M3BoardStateModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3BoardStateModel::PushUnusedElement(const M3ElementModel_SharedPtr& Element) {
	const auto& UnusedElements = Entity->Get()->UnusedElements->Get();
	const auto It = std::find_if(UnusedElements->begin(), UnusedElements->end(), [=](const M3ElementModel_SharedPtr& Value) {
		return Value == Element;
	});
	if (It == UnusedElements->end()) {
		Element->Reset();
		UnusedElements->push_back(Element);
	} else {
		assert(false);
	}
}

M3ElementModel_SharedPtr M3BoardStateModel::PopUnusedElement() {
	M3ElementModel_SharedPtr Element = nullptr;
	const auto& UnusedElements = Entity->Get()->UnusedElements->Get();
	if (UnusedElements->size() != 0) {
		Element = UnusedElements->back();
		UnusedElements->pop_back();
	}
	return Element;
}
