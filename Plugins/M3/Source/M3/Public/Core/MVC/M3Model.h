// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Entity.h"
#include "M3KVListener.h"
#include "M3AppEvent.h"
#include "M3GlobalDispatcher.h"

FORWARD_DECL_STRONG(AM3Scheme_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)

class M3_API M3ModelContainer_INTERFACE {
public:

	M3ModelContainer_INTERFACE() = default;
	virtual ~M3ModelContainer_INTERFACE() = default;

	static std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> Container(uintptr_t Guid);
	static std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> TempContainer(uintptr_t Guid);
};

class M3_API M3ModelPool_INTERFACE {
public:

	M3ModelPool_INTERFACE() = default;
	virtual ~M3ModelPool_INTERFACE() = default;

	static std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> Pool(uintptr_t Guid);
};

class M3_API M3ModelComponent_INTERFACE {
protected:

	static std::set<uintptr_t> GuidsContainer;

public:

	CTTI_CLASS_GUID(M3ModelComponent_INTERFACE, M3ModelComponent_INTERFACE::GuidsContainer)

	M3ModelComponent_INTERFACE() = default;
	virtual ~M3ModelComponent_INTERFACE() = default;
};

class M3_API M3Model_INTERFACE : public M3KVListener_INTERFACE {
protected:

	static std::set<uintptr_t> GuidsContainer;

	static std::array<std::list<M3Model_INTERFACE_SharedPtr>, std::numeric_limits<uint8_t>::max()> Pool;

	std::weak_ptr<M3Model_INTERFACE> Parent;
	std::array<std::shared_ptr<M3Model_INTERFACE>, std::numeric_limits<uint8_t>::max()> Submodels;
	
	std::array<std::shared_ptr<M3ModelComponent_INTERFACE>, std::numeric_limits<uint8_t>::max()> Components;

public:

	M3Model_INTERFACE() = default;
	virtual ~M3Model_INTERFACE() = default;

	CTTI_CLASS_GUID(M3Model_INTERFACE, M3Model_INTERFACE::GuidsContainer)

	virtual void Init() = 0;
	virtual void Serialize() = 0;
	virtual void Deserialize(AM3Scheme_INTERFACE* Scheme) = 0;
	virtual void Reset() = 0;

	void AddComponent(const std::shared_ptr<M3ModelComponent_INTERFACE>& Component) {
		Components[Component->InstanceGuid()] = Component;
	};

	void RemoveComponent(const std::shared_ptr<M3ModelComponent_INTERFACE>& Component) {
		Components[Component->InstanceGuid()] = nullptr;
	};

	void RemoveAllComponents() {
		for (const auto Component : Components) {
			if (Component) {
				Components[Component->InstanceGuid()] = nullptr;
			}
		}
	};

	template<typename TComponent>
	std::shared_ptr<TComponent> GetComponent() const {
		static_assert(std::is_base_of<M3ModelComponent_INTERFACE, TComponent>::value, "TComponent must derive from M3ModelComponent_INTERFACE");
		return std::static_pointer_cast<TComponent>(Components[TComponent::ClassGuid()]);
	};

	template<typename TModel>
	std::shared_ptr<TModel> GetParent() {
		static_assert(std::is_base_of<M3Model_INTERFACE, TModel>::value, "TModel must derive from M3Model_INTERFACE");
		std::shared_ptr<TModel> Result = nullptr;
		if (!Parent.expired()) {
			Result = std::static_pointer_cast<TModel>(Parent.lock());
		}
		return Result;
	};

	template<typename TModel>
	void SetParent(const std::shared_ptr<TModel>& _Parent) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TModel>::value, "TModel must derive from M3Model_INTERFACE");
		Parent = _Parent;
	};

	template<typename TSubmodel>
	void AddSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		if (!Submodels[Submodel->InstanceGuid()]) {
			Submodels[Submodel->InstanceGuid()] = Submodel;
			const auto Instance = M3Model_INTERFACE::shared_from_this();
			Submodel->SetParent(std::static_pointer_cast<M3Model_INTERFACE>(Instance));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Can't add same model to Shared Model"));
			assert(false);
		}
	};

	template<typename TSubmodel>
	void RemoveSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		Submodel->ResetParent();
		Submodels[Submodel->InstanceGuid()] = nullptr;
	};

	void RemoveAllSubmodels() {
		std::for_each(Submodels.begin(), Submodels.end(), [=](const M3Model_INTERFACE_SharedPtr& Submodel) {
			if (Submodel) {
				RemoveSubmodel(Submodel);
			}
		});
	};

	template<typename TSubmodel>
	std::shared_ptr<TSubmodel> GetSubmodel() const {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		return std::static_pointer_cast<TSubmodel>(Submodels[TSubmodel::ClassGuid()]);
	};

	std::array<std::shared_ptr<M3Model_INTERFACE>, std::numeric_limits<uint8_t>::max()>& GetSubmodels() {
		return Submodels;
	};

	void ResetParent() {
		Parent.reset();
	};
};

template<typename T>
class M3_API M3Model : public M3Model_INTERFACE {
static_assert(std::is_base_of<M3Entity, T>::value, "T must derive from M3Entity");

public:

	CTTI_CLASS_GUID(M3Model<T>, M3Model<T>::GuidsContainer)

	PROP_STRONG(public, M3Model<T>, Entity, std::shared_ptr<T>, nullptr)

	M3Model() {
		Entity->Set(std::make_shared<T>());
		Parent.reset();
		Submodels.fill(nullptr);
	};
	virtual ~M3Model() {
		Parent.reset();
		Submodels.fill(nullptr);
	};

	template<typename RESULT>
	static std::shared_ptr<RESULT> Construct() {
		std::shared_ptr<RESULT> Result = nullptr;
		const auto Pool = M3ModelPool_INTERFACE::Pool(ClassGuid());
		if (Pool->size() != 0) {
			Result = std::static_pointer_cast<RESULT>(Pool->back());
			Pool->pop_back();
		} else {
			Result = std::make_shared<RESULT>();
			Result->Init();
		}
		Result->AddToContainer();
		return Result;
	}

	static void Destruct(const std::shared_ptr<M3Model<T>>& Model) {
		const auto Submodels = Model->GetSubmodels();
		std::for_each(Submodels.begin(), Submodels.end(), [=](const M3Model_INTERFACE_SharedPtr& Submodel) {
			if (Submodel) {
				const auto SubmodelTemp = std::static_pointer_cast<M3Model>(Submodel);
				Model->RemoveSubmodel(std::static_pointer_cast<M3Model>(Submodel));
				Destruct(SubmodelTemp);
			}
		});

		const auto Pool = M3ModelPool_INTERFACE::Pool(Model->InstanceGuid());
		Pool->push_back(Model);

		Model->RemoveFromContainer();
		Model->Reset();
	};
	
	virtual void Init() override {

	};

	virtual void Reset() override {
		Entity->Get()->IsAssignedToView->Set(false);
	};

	static std::shared_ptr<M3KVProperty<std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>>>> Container() {
		return M3ModelContainer_INTERFACE::Container(ClassGuid());
	};

	static std::shared_ptr<std::list<M3Model_INTERFACE_SharedPtr>> TempContainer() {
		return M3ModelContainer_INTERFACE::TempContainer(ClassGuid());
	};

	static void ApplyContainer() {
		const auto& Container = M3ModelContainer_INTERFACE::Container(ClassGuid());
		const auto& TempContainer = M3ModelContainer_INTERFACE::TempContainer(ClassGuid());
		Container->Set(TempContainer);
	};

	void AddToContainer() {
		const auto& Instance = std::static_pointer_cast<M3Model<T>>(M3Model_INTERFACE::shared_from_this());
		const auto& Container = M3ModelContainer_INTERFACE::TempContainer(Instance->InstanceGuid());
		ensure(Container != nullptr);
		Container->push_back(Instance);
	};

	void RemoveFromContainer() {
		const auto& Instance = std::static_pointer_cast<M3Model<T>>(M3Model_INTERFACE::shared_from_this());
		const auto& Container = M3ModelContainer_INTERFACE::TempContainer(Instance->InstanceGuid());
		ensure(Container != nullptr);
		Container->remove(Instance);
	};

	void Subscribe(const M3KVProperty_INTERFACE_SharedPtr& Prop, const std::string& PropId) {
		const auto Instance = M3KVListener_INTERFACE::shared_from_this();
		Prop->SetId(PropId);
		Prop->Subscribe(Instance);
	};

	void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop) {
		const auto& ModelInstance = std::static_pointer_cast<M3Model_INTERFACE>(shared_from_this());
		M3GlobalDispatcher::GetInstance()->Publish<M3AppEventModelProp>(M3AppEventModelProp::GenerateGuid(InstanceGuid(), Prop->GetId()), ModelInstance, Prop);
	};
};

#define SUBSCRIBE_PROP(__class__, __prop__) \
Subscribe(Entity->Get()->__prop__, __class__::PROPERTY_ID_##__prop__()); \

