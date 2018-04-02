// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include <functional>
#include "CoreMinimal.h"
#include "M3KVListener.h"

template<typename T>
class M3KVConnection : public M3KVListener<T>
{
private:

	std::function<void(const T&)> Callback;

	void Call(const T& Value) {
		return Callback(Value);
	};

public:

	M3KVConnection(const std::function<void(const T&)>& _Callback) {
		Callback = _Callback;
	};

	void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop, const T& Value) {
		Call(Value);
	};
};
