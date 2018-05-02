// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3KVDispatcher.h"
#include "M3KVCoding.h"
#include "M3Utilities.h"
#include "M3KVProperty.generated.h"

FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Core))
class M3_API UM3KVProperty_INTERFACE : public UObject {
private:

	GENERATED_BODY()

public:

	virtual ~UM3KVProperty_INTERFACE() = default;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Core))
class M3_API UM3KVListener_INTERFACE : public UObject {
private:

	GENERATED_BODY()

public:

	virtual ~UM3KVListener_INTERFACE() = default;

	virtual void OnChanged(const UM3KVProperty_INTERFACE* Prop) { };
};

class M3KVListener_INTERFACE : public std::enable_shared_from_this<M3KVListener_INTERFACE>
{
protected:

public:

	virtual ~M3KVListener_INTERFACE() = default;
	virtual void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop) = 0;
};

class M3KVProperty_INTERFACE : public M3KVDispatcher, public std::enable_shared_from_this<M3KVProperty_INTERFACE> {
protected:

	std::string Id;

public:

	M3KVProperty_INTERFACE() {
		Id = "Unknown";
	};

	virtual ~M3KVProperty_INTERFACE() = default;

	std::string GetId() const {
		return Id;
	};

	void SetId(const std::string& _Id) {
		this->Id = _Id;
	};
};
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)

template<typename T>
class M3KVProperty : public M3KVCoding<T>, public M3KVProperty_INTERFACE
{
private:

	T Value;

protected:

	void Init() {
		M3KVCoding<T>::Getter = [=]() {
			return Value;
		};

		M3KVCoding<T>::Setter = [=](const T& _Value) {
			Value = _Value;
			if (Listeners.size() != 0) {
				for (const auto& Listener : Listeners) {
					Listener->OnChanged(shared_from_this());
				}
			}
		};
	};

public:

	M3KVProperty() {
		Init();
	};

	M3KVProperty(const T& _Value) {
		Value = _Value;
		Init();
	}

	virtual ~M3KVProperty() = default;

	M3KVProperty(const M3KVProperty&) = delete;
	M3KVProperty& operator= (const M3KVProperty&) = delete;

	void Set(const T& _Value) {
		if (!M3KVCoding<T>::Setter) {
			assert(false);
		}
		M3KVCoding<T>::Setter(_Value);
	}

	T Get() const {
		if (!M3KVCoding<T>::Getter) {
			assert(false);
		}
		return M3KVCoding<T>::Getter();
	}
};