// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
class M3KVCoding {
protected:

	std::function<T()> Getter = nullptr;
	std::function<void(const T&)> Setter = nullptr;

public:

	M3KVCoding() {
		Getter = nullptr;
		Setter = nullptr;
	}

	virtual ~M3KVCoding() = default;

	virtual void Set(const T& _Value) = 0;
	virtual T Get() const = 0;
};
