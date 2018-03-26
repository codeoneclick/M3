// Fill out your copyright notice in the Description page of Project Settings.

#include "M3CellModel.h"
#include "M3ElementModel.h"

M3CellModel::M3CellModel() {

}

M3CellModel::~M3CellModel() {

}

void M3CellModel::Init() {

}

void M3CellModel::Serialize() {

}

void M3CellModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

bool M3CellModel::IsContainElement() {
	bool Result = false;
	const auto Element = GetSubmodel<M3ElementModel>();
	if (Element && !GetIsClosed()) {
		Result = true;
	}
	return Result;
}

bool M3CellModel::CanContainElement() {
	bool Result = false;
	if (!IsContainElement() && !GetIsClosed()) {
		Result = true;
	}
	return Result;
}
