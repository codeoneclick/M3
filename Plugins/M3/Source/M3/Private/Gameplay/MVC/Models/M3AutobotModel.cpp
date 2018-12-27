// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3AutobotModel.h"
#include "M3SuperElementModel.h"
#include "M3BoardModel.h"
#include "M3ElementModel.h"
#include "M3SharedModel.h"

FORWARD_DECL_STRONG(M3ElementModel)

M3AutobotModel::M3AutobotModel() {
}

M3AutobotModel::~M3AutobotModel() {
}

void M3AutobotModel::Init() {

}

void M3AutobotModel::Serialize() {

}

void M3AutobotModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

float M3AutobotModel::GetMatchWeightning(int Count, int HorizonatlStart, int HorizontalEnd, int VerticalStart, int VerticalEnd, int Col, int Row) {
	float Result = Entity->Get()->MatchWeightning->Get() * Count;
	return Result;
}

float M3AutobotModel::GetSuperElementWeightning(EM3ElementId SuperElementId, int Col, int Row, int* OutColToSwap, int* OutRowToSwap, std::shared_ptr<std::set<M3ElementModel_SharedPtr>> IgnoredRegularElements, std::shared_ptr<std::set<M3ElementModel_SharedPtr>> IgnoredSuperElements) {
	float Result = Entity->Get()->MatchWeightning->Get();
	int Radius = M3SuperElementModel::GetExplosionRadius(SuperElementId);
	const auto& BoardModel = M3SharedModel::GetInstance()->GetSubmodel<M3BoardModel>();

	if (!IgnoredRegularElements) {
		IgnoredRegularElements = std::make_shared<std::set<M3ElementModel_SharedPtr>>();
	}

	if (!IgnoredSuperElements) {
		IgnoredSuperElements = std::make_shared<std::set<M3ElementModel_SharedPtr>>();
	}

	*OutColToSwap = Col;
	*OutRowToSwap = Row;

	M3ElementModel_SharedPtr ElementModel = nullptr;
	for (int i = Col - Radius; i <= Col + Radius; ++i) {
		for (int j = Row - Radius; j <= Row + Radius; ++j) {
			if ((i == Col - Radius && j == Row - Radius) ||
				(i == Col - Radius && j == Row + Radius) ||
				(i == Col + Radius && j == Row - Radius) ||
				(i == Col + Radius && j == Row + Radius)) continue;

			ElementModel = BoardModel->GetElement(i, j);
			if (ElementModel) {
				bool bIsSuperelementsChain = false;
				if (i != Col || j != Row) {
					if (ElementModel->IsSuper() && ElementModel->CanMatch() && IgnoredSuperElements->count(ElementModel) == 0) {
						bIsSuperelementsChain = true;
						Result += Entity->Get()->MatchWeightning->Get();
						IgnoredSuperElements->insert(ElementModel);
					}
				}

				if (ElementModel->IsRegular() &&  ElementModel->CanMatch() && IgnoredRegularElements->count(ElementModel)) {
					Result += Entity->Get()->MatchWeightning->Get();
					IgnoredRegularElements->insert(ElementModel);
				}

				if (bIsSuperelementsChain) {
					int OutColToSwapTemp = 0;
					int OutRowToSwapTemp = 0;
					const auto& SuperElementModel = ElementModel->GetSubmodel<M3SuperElementModel>();
					Result += GetSuperElementWeightning(SuperElementModel->GetId(), i, j, &OutColToSwapTemp, &OutRowToSwapTemp, IgnoredRegularElements, IgnoredSuperElements);
				}
			}
		}
	}

	return Result;
}
