// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "GameFramework/Actor.h"
#include "M3ViewDelegates_API.h"

FORWARD_DECL_STRONG(M3KVSlot_INTERFACE)
FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3View_INTERFACE)
FORWARD_DECL_STRONG(AM3AssetsBundle)
FORWARD_DECL_STRONG(AM3ViewDelegates_API)
FORWARD_DECL_WEAK(M3View_INTERFACE)
FORWARD_DECL_STRONG(UM3ViewDelegate_INTERFACE)

class HEXMAP_API M3View_INTERFACE : public std::enable_shared_from_this<M3View_INTERFACE> {
protected:

	static std::set<uintptr_t> GuidsContainer;

	M3View_INTERFACE_WeakPtr Parent;
	std::vector<M3View_INTERFACE_SharedPtr> Subviews;

	M3Model_INTERFACE_SharedPtr ViewModel = nullptr;
	AM3AssetsBundle* Bundle = nullptr;

	AM3ViewDelegates_API* Delegates = nullptr;
	UM3ViewDelegate_INTERFACE* Delegate = nullptr;

public:

	CTTI_CLASS_GUID(M3View_INTERFACE, M3View_INTERFACE::GuidsContainer)

	M3View_INTERFACE() = default;
	virtual ~M3View_INTERFACE() = default;

	virtual void Load(AM3AssetsBundle* Bundle) = 0;
	virtual void BindViewDelegates(AM3ViewDelegates_API* API) = 0;
	virtual void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel) = 0;

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
};

class HEXMAP_API M3View : public M3View_INTERFACE {
protected:

	AActor* Superview = nullptr;
	std::unordered_map<std::string, M3KVSlot_INTERFACE_SharedPtr> Slots;

public:

	CTTI_CLASS_GUID(M3View, M3View_INTERFACE::GuidsContainer)
	
	M3View(AActor* Superview);
	virtual ~M3View();
	
	virtual void Load(AM3AssetsBundle* Bundle);
	virtual void BindViewDelegates(AM3ViewDelegates_API* API);
	virtual void BindViewModel(const M3Model_INTERFACE_SharedPtr& ViewModel);

	template<typename T> 
	std::shared_ptr<T> GetViewModel() const {
		return std::static_pointer_cast<T>(ViewModel);
	};
	
	template<typename T>
	T* GetBundle() const {
		return static_cast<T*>(Bundle);
	};

	AActor* GetSuperview() const;

	void AddSubview(const M3View_INTERFACE_SharedPtr& Subview);
	void RemoveSubview(const M3View_INTERFACE_SharedPtr& Subview);
	void RemoveFromParent();
};
