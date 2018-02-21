// Fill out your copyright notice in the Description page of Project Settings.

#include "M3BoardModel.h"
#include "M3CellModel.h"
#include "M3ElementModel.h"

FORWARD_DECL_STRONG(M3CellModel)
FORWARD_DECL_STRONG(M3ElementModel)

M3BoardModel::M3BoardModel()
{
	M3CellModel::Register();
	M3ElementModel::Register();
}

M3BoardModel::~M3BoardModel()
{
}

void M3BoardModel::Init() {
	SUBSCRIBE_PROP(M3BoardEntity, Cols)
	SUBSCRIBE_PROP(M3BoardEntity, Rows)
	SUBSCRIBE_PROP(M3BoardEntity, Cells)
}

void M3BoardModel::Serialize() {

}

void M3BoardModel::Deserialize() {
	int Cols = 8;
	int Rows = 8;
	Entity->Get()->Cols->Set(Cols);
	Entity->Get()->Rows->Set(Rows);

	for (int i = 0; i < Cols; ++i) {
		for (int j = 0; j < Rows; ++j) {
			M3CellModel_SharedPtr CellModel = std::make_shared<M3CellModel>();
			CellModel->Init();
			CellModel->AddToContainer();

			M3ElementModel_SharedPtr ElementModel = std::make_shared<M3ElementModel>();
			ElementModel->Init();
			ElementModel->AddToContainer();
		}
	}
 	M3CellModel::ApplyContainer();
	M3ElementModel::ApplyContainer();
}

