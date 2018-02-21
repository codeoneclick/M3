// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3KVSlot.h"

template <typename T>
class M3KVMultiSlot : public M3KVSlot_INTERFACE
{
private:

	std::vector<std::pair<std::shared_ptr<M3KVProperty<T>>, std::shared_ptr<M3KVConnection<T>>>> Connections;

public:

	M3KVMultiSlot() = default;
	~M3KVMultiSlot() = default;

	std::shared_ptr<M3KVConnection<T>> Attach(const std::shared_ptr<M3KVProperty<T>> Prop, const std::function<void(const T&)>& Callback) {
		std::shared_ptr<M3KVConnection<T>> Connection = nullptr;
		if (Callback) {
			Connection = std::make_shared<M3KVConnection<T>>(Callback);
			Connections.push_back(std::make_pair(Prop, Connection));
			Prop->Subscribe(Connection);
		}
		return Connection;
	};

	void Detach(const std::shared_ptr<M3KVConnection<T>>& Connection) {
		std::remove_if(Connections.begin(), Connections.end(), [Connection](const std::pair<std::shared_ptr<M3KVProperty<T>>, std::shared_ptr<M3KVConnection<T>>>& It) {
			if (It.second == Connection) {
				It.first->Unsubscribe(Connection);
			}
			return It.second == Connection;
		});
	};

	void DetachAll() {
		for (const auto& It : Connections) {
			It->first->Unsubscribe(It->second);
		}
		Connections.clear();
	};
};
