// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3KVProperty.h"

template<typename T>
class M3KVListener : public M3KVListener_INTERFACE
{
private:

public:

	virtual ~M3KVListener() = default;
	virtual void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop) {
		OnChanged(Prop, std::static_pointer_cast<M3KVProperty<T>>(Prop)->Get());
	};
	virtual void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop, const T& Value) = 0;
};
