// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Entity.h"
#include "M3KVListener.h"
#include "M3AppEvent.h"
#include "M3GlobalDispatcher.h"

FORWARD_DECL_STRONG(AM3Scheme_INTERFACE)

class M3_API M3ModelComponent_INTERFACE {
protected:

	static std::set<uintptr_t> GuidsContainer;

public:

	M3ModelComponent_INTERFACE() = default;
	virtual ~M3ModelComponent_INTERFACE() = default;

	CTTI_CLASS_GUID(M3ModelComponent_INTERFACE, M3ModelComponent_INTERFACE::GuidsContainer)
};

class M3_API M3Model_INTERFACE : public M3KVListener_INTERFACE {
protected:

	static std::set<uintptr_t> GuidsContainer;

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
};

template<typename T>
class M3_API M3Model : public M3Model_INTERFACE
{
static_assert(std::is_base_of<M3Entity, T>::value, "T must derive from M3Entity");
private:

	PROP_STATIC(public, M3Model<T>, TempContainer, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>)
	PROP_STATIC(public, M3Model<T>, Pool, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>)

protected:

	std::array<std::shared_ptr<M3Model_INTERFACE>, std::numeric_limits<uint8_t>::max()> Submodels;

public:

	CTTI_CLASS_GUID(M3Model<T>, M3Model<T>::GuidsContainer)

	PROP_STRONG(public, M3Model<T>, Entity, std::shared_ptr<T>, nullptr)

	PROP_STATIC(public, M3Model<T>, Container, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>)

	PROP_WEAK(public, M3Model<T>, Parent, std::weak_ptr<M3Model_INTERFACE>)

	M3Model() {
		Entity->Set(std::make_shared<T>());
		Submodels.fill(nullptr);
	};
	virtual ~M3Model() {
		Parent->Get().reset();
		Submodels.fill(nullptr);
	};

	template<typename RESULT>
	static std::shared_ptr<RESULT> Construct() {
		std::shared_ptr<RESULT> Result = nullptr;
		if (Pool->Get()->size() != 0) {
			Result = std::static_pointer_cast<RESULT>(Pool->Get()->back());
			Pool->Get()->pop_back();
		} else {
			Result = std::make_shared<RESULT>();
		}
		Result->Init();
		Result->AddToContainer();
		return Result;
	}
	
	virtual void Init() override {

	};

	virtual void Reset() override {
		std::for_each(Submodels.begin(), Submodels.end(), [=](const M3Model_INTERFACE_SharedPtr& Submodel) {
			if (Submodel) {
				Submodel->Reset();
				RemoveSubmodel(std::static_pointer_cast<M3Model>(Submodel));
			}
		});
		Pool->Get()->push_back(std::static_pointer_cast<M3Model>(shared_from_this()));
		RemoveFromContainer();
	}

	static void Register() {
		auto Storage = std::make_shared<std::vector<std::shared_ptr<M3Model<T>>>>();
		Container = std::make_shared<M3KVProperty<decltype(Storage)>>(Storage);
		Storage = std::make_shared<std::vector<std::shared_ptr<M3Model<T>>>>();
		TempContainer = std::make_shared<M3KVProperty<decltype(Storage)>>(Storage);
		Storage = std::make_shared<std::vector<std::shared_ptr<M3Model<T>>>>();
		Pool = std::make_shared<M3KVProperty<decltype(Storage)>>(Storage);
	};

	static void Unregister() {
		if (TempContainer->Get()) {
			TempContainer->Get()->clear();
		}
		if (Container->Get()) {
			Container->Get()->clear();
		}
		if (Pool->Get()) {
			Pool->Get()->clear();
		}
	};

	static void ApplyContainer() {
		Container->Set(TempContainer->Get());
	};

	void AddToContainer() {
		const auto Instance = M3Model_INTERFACE::shared_from_this();
		TempContainer->Get()->push_back(std::static_pointer_cast<M3Model<T>>(Instance));
	};

	void RemoveFromContainer() {
		const auto Instance = M3Model_INTERFACE::shared_from_this();
		std::remove_if(TempContainer->Get()->begin(), TempContainer->Get()->end(), [=](const std::shared_ptr<M3Model<T>> Model) {
			return Instance == Model;
		});
	}

	template<typename TSubmodel>
	void AddSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		if (!Submodels[Submodel->InstanceGuid()]) {
			Submodels[Submodel->InstanceGuid()] = Submodel;
			const auto Instance = M3Model_INTERFACE::shared_from_this();
			Submodel->Parent->Set(std::static_pointer_cast<M3Model<T>>(Instance));
		} else {
			UE_LOG(LogTemp, Error, TEXT("Can't add same model to Shared Model"));
			assert(false);
		}
	};

	template<typename TSubmodel>
	void RemoveSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		Submodels[Submodel->InstanceGuid()] = nullptr;
		Submodel->Parent->Get().reset();
	};

	void RemoveAllSubmodels() {
		std::for_each(Submodels.begin(), Submodels.end(), [=](const M3Model_INTERFACE_SharedPtr& Submodel) {
			if (Submodel) {
				RemoveSubmodel(std::static_pointer_cast<M3Model>(Submodel));
			}
		});
	};

	template<typename TSubmodel>
	std::shared_ptr<TSubmodel> GetSubmodel() const {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		return std::static_pointer_cast<TSubmodel>(Submodels[TSubmodel::ClassGuid()]);
	};

	template<typename TModel>
	std::shared_ptr<TModel> GetParent() {
		static_assert(std::is_base_of<M3Model_INTERFACE, TModel>::value, "TModel must derive from M3Model_INTERFACE");
		std::shared_ptr<TModel> Result = nullptr;
		if (!Parent->Get().expired()) {
			Result = std::static_pointer_cast<TModel>(Parent->Get().lock());
		}
		return Result;
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

template<typename T>
PROP_STATIC_INIT(M3Model<T>, Container, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>, nullptr)

template<typename T>
PROP_STATIC_INIT(M3Model<T>, TempContainer, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>, nullptr)

template<typename T>
PROP_STATIC_INIT(M3Model<T>, Pool, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>, nullptr)

#define SUBSCRIBE_PROP(__class__, __prop__) \
Subscribe(Entity->Get()->__prop__, __class__::PROPERTY_ID_##__prop__()); \

