// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3Model.h"
#include "M3SharedModel.h"

std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> M3ModelContainer_INTERFACE::Container(uintptr_t Guid) {
	return M3SharedModel::GetInstance()->Container(Guid);
}

std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> M3ModelContainer_INTERFACE::TempContainer(uintptr_t Guid) {
	return M3SharedModel::GetInstance()->TempContainer(Guid);
}

std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> M3ModelPool_INTERFACE::Pool(uintptr_t Guid) {
	return M3SharedModel::GetInstance()->Pool(Guid);
}

std::set<uintptr_t> M3ModelComponent_INTERFACE::GuidsContainer;
std::set<uintptr_t> M3Model_INTERFACE::GuidsContainer;
