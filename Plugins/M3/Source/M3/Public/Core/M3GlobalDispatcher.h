// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

FORWARD_DECL_STRONG(M3GlobalDispatcher)
FORWARD_DECL_STRONG(M3AppEvent_INTERFACE)

class M3_API M3GlobalDispatcher
{
private:

	static M3GlobalDispatcher_SharedPtr Instance;

	std::unordered_map<std::string, std::vector<M3AppEvent_INTERFACE_SharedPtr>> Listeners;

public:

	M3GlobalDispatcher() = default;
	~M3GlobalDispatcher() = default;

	static M3GlobalDispatcher_SharedPtr GetInstance() {
		if (Instance == nullptr) {
			Instance = std::make_shared<M3GlobalDispatcher>();
		}
		return Instance;
	};

	template<typename T, class... ARGS>
	void Publish(const std::string& Id, ARGS... Args) {
		const auto It = Listeners.find(Id);
		if (It != Listeners.end()) {
			for (const auto& Event : It->second) {
				std::static_pointer_cast<T>(Event)->Call(std::forward<ARGS>(Args)...);
			}
		}
	};

	void Subscribe(M3AppEvent_INTERFACE_SharedPtr Event);
	void Unsubscribe(M3AppEvent_INTERFACE_SharedPtr Event);
};
