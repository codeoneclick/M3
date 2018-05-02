// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Model.h"
#include "M3Entity.h"

class M3SharedEntity : public M3Entity {
public:

	CTTI_CLASS_GUID(M3SharedEntity)

	PROP_STRONG(public, M3SharedEntity, Version, int, 0)
};

FORWARD_DECL_STRONG(M3SharedModel)
FORWARD_DECL_STRONG(M3Model_INTERFACE)

class M3_API M3SharedModel : public M3Model<M3SharedEntity>
{
private:

	static M3SharedModel_SharedPtr Instance;

protected:

	std::unordered_map<uintptr_t, std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>> Pools;

	std::unordered_map<uintptr_t, std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>> Containers;
	std::unordered_map<uintptr_t, std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>> TempContainers;

public:

	M3SharedModel();
	~M3SharedModel();

	static M3SharedModel_SharedPtr GetInstance();

	CTTI_CLASS_GUID(M3SharedModel)

	void Init();

	void Serialize();
	void Deserialize(AM3Scheme_INTERFACE* Scheme);

	template<typename T>
	void RegisterContainer() {
		ensure(Containers[T::ClassGuid()] == nullptr);
		Containers[T::ClassGuid()] = std::make_shared<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>>(std::make_shared<std::list<M3Model_INTERFACE_SharedPtr>>());
		
		ensure(TempContainers[T::ClassGuid()] == nullptr);
		TempContainers[T::ClassGuid()] = std::make_shared<std::list<M3Model_INTERFACE_SharedPtr>>();

		ensure(Pools[T::ClassGuid()] == nullptr);
		Pools[T::ClassGuid()] = std::make_shared<std::list<M3Model_INTERFACE_SharedPtr>>();
	};

	template<typename T>
	void UnregisterContainer() {
		ensure(Containers[T::ClassGuid()] != nullptr);
		Containers[T::ClassGuid()]->Get()->clear();
		Containers[T::ClassGuid()] = nullptr;

		ensure(TempContainers[T::ClassGuid()] != nullptr);
		TempContainers[T::ClassGuid()]->clear();
		TempContainers[T::ClassGuid()] = nullptr;

		ensure(Pools[T::ClassGuid()] == nullptr);
		Pools[T::ClassGuid()]->clear();
		Pools[T::ClassGuid()] = nullptr;
	};

	void UnregisterAllContainers();

	template<typename T>
	std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> Container() {
		return Container(T::ClassGuid());
	};

	template<typename T>
	std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> TempContainer() {
		return TempContainer(T::ClassGuid());
	};

	template<typename T>
	std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> Pool() {
		return Pool(T::ClassGuid());
	};

	std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> Container(uintptr_t Guid);
	std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> TempContainer(uintptr_t Guid);

	std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> Pool(uintptr_t Guid);

	template<typename TModel>
	void Deserialize(AM3Scheme_INTERFACE* Scheme) {
		const auto& It = Submodels.find(TModel::ClassGuid());
		if (It != Submodels.end()) {
			It->second->Deserialize(Scheme);
		}
	};
};
