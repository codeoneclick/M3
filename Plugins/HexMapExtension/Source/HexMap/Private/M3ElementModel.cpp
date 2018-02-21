// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementModel.h"

M3ElementModel::M3ElementModel()
{
}

M3ElementModel::~M3ElementModel()
{
}

void M3ElementModel::Init() {
	SUBSCRIBE_PROP(M3ElementEntity, ElementId)
	SUBSCRIBE_PROP(M3ElementEntity, State)
}

void M3ElementModel::Serialize() {

}

void M3ElementModel::Deserialize() {

}
