// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3ViewDelegate.h"
#include "M3ViewAccessor.h"

FORWARD_DECL_STRONG(M3KVSlot_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3AssetsBundle)
FORWARD_DECL_STRONG(UM3ViewDelegate_INTERFACE)
FORWARD_DECL_STRONG(UM3ViewAccessor_INTERFACE)
FORWARD_DECL_WEAK(M3View_INTERFACE)

class M3_API M3View_INTERFACE : public std::enable_shared_from_this<M3View_INTERFACE> {
protected:

	M3View_INTERFACE_WeakPtr Parent;
	std::vector<M3View_INTERFACE_SharedPtr> Subviews;

	M3Model_INTERFACE_SharedPtr ViewModel = nullptr;
	UM3AssetsBundle* Bundle = nullptr;

	UM3ViewDelegate_INTERFACE* Delegate = nullptr;
	UM3ViewAccessor_INTERFACE* Accessor = nullptr;

public:

	CTTI_CLASS_GUID(M3View_INTERFACE)

	M3View_INTERFACE() = default;
	virtual ~M3View_INTERFACE() = default;

	virtual void Load(UM3AssetsBundle* Bundle) = 0;
	virtual void BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel) = 0;
	virtual void BindViewDelegate(UM3ViewDelegate_INTERFACE* _Delegate) = 0;
	virtual void BindViewAccessor(UM3ViewAccessor_INTERFACE* _Accessor) = 0;

	template<typename T>
	std::vector<std::shared_ptr<T>> GetSubviews() const {
		std::vector<std::shared_ptr<T>> Result;
		for (const auto& It : this->Subviews) {
			if (It->InstanceGuid() == T::ClassGuid()) {
				Result.push_back(std::static_pointer_cast<T>(It));
			}
		}
		return Result;
	};

	template<typename T>
	T* GetDelegate() const {
		T* Result = static_cast<T*>(Delegate);
		return Result;
	};

	template<typename T> 
	T* GetAccessor() const {
		T* Result = static_cast<T*>(Accessor);
		return Result;
	}

	void AddSubview(const M3View_INTERFACE_SharedPtr& Subview);
	void RemoveSubview(const M3View_INTERFACE_SharedPtr& Subview);
	void RemoveFromParent();
};

class M3_API M3View : public M3View_INTERFACE {
protected:

	AActor* Superview = nullptr;

	std::unordered_map<std::string, M3KVSlot_INTERFACE_SharedPtr> Slots;

public:

	CTTI_CLASS_GUID(M3View)
	
	M3View(AActor* Superview);
	virtual ~M3View();
	
	virtual void Load(UM3AssetsBundle* Bundle);
	virtual void BindViewModel(const M3Model_INTERFACE_SharedPtr& _ViewModel);
	virtual void BindViewDelegate(UM3ViewDelegate_INTERFACE* _Delegate);
	virtual void BindViewAccessor(UM3ViewAccessor_INTERFACE* _Accessor);

	template<typename T> 
	std::shared_ptr<T> GetViewModel() const {
		return std::static_pointer_cast<T>(ViewModel);
	};
	
	template<typename T>
	T* GetBundle() const {
		return static_cast<T*>(Bundle);
	};

	template<typename T>
	void Dispose() {
		const auto CurrentSuperview = static_cast<T*>(GetSuperview());
		if (Delegate) {
			Delegate->Dispose();
			Delegate = nullptr;
		}

		if (Accessor) {
			Accessor->Dispose();
			Accessor = nullptr;
		}

		CurrentSuperview->Dispose();
		CurrentSuperview->Destroy();
		RemoveFromParent();
	};

	AActor* GetSuperview() const;
};
