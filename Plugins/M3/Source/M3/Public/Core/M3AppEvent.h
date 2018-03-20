// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

template<unsigned int NUM>
struct M3AppEvent_Params
{
	template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
	static void Forward(std::function<void(F_ARGS... Args)> Function, std::tuple<T_ARGS...> const& TArgs, ARGS... Args)
	{
		M3AppEvent_Params<NUM - 1>::Forward(Function, TArgs, std::get<NUM - 1>(TArgs), Args...);
	}
};

template<>
struct M3AppEvent_Params<0>
{
	template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
	static void Forward(std::function<void(F_ARGS... args)> Function, std::tuple<T_ARGS...> const&, ARGS... Args)
	{
		Function(Args...);
	}
};

template<typename... F_ARGS, typename... T_ARGS>
void Forward(std::function<void(F_ARGS... FArgs)> Function, std::tuple<T_ARGS...> const& TArgs)
{
	M3AppEvent_Params<sizeof...(T_ARGS)>::Forward(Function, TArgs);
};

class M3AppEvent_Callback_INTERFACE : std::enable_shared_from_this<M3AppEvent_Callback_INTERFACE>
{
public:

	virtual ~M3AppEvent_Callback_INTERFACE() = default;
};
 
template<class FUCTION, class... ARGS>
class M3AppTask_Callback : public M3AppEvent_Callback_INTERFACE
{
protected:

	FUCTION Function;
	std::tuple<ARGS...> Args;

public:

	M3AppTask_Callback(FUCTION Function, ARGS... Args)
	{
		this->Function = Function;
		this->Args = std::make_tuple(Args...);
	};

	virtual ~M3AppTask_Callback() = default;
	void Call() {
		Forward(Function, Args);
	};
};

FORWARD_DECL_STRONG(M3AppEvent_Callback_INTERFACE)

class M3AppEvent_INTERFACE : public std::enable_shared_from_this<M3AppEvent_INTERFACE>
{
protected:

	std::string Id;
	M3AppEvent_Callback_INTERFACE_SharedPtr Callback;

public:

	M3AppEvent_INTERFACE(const std::string& Id, const M3AppEvent_Callback_INTERFACE_SharedPtr& Callback) {
		this->Id = Id;
		this->Callback = Callback;
	}
	
	virtual ~M3AppEvent_INTERFACE() = default;

	std::string GetId() const {
		return Id;
	}
};

FORWARD_DECL_STRONG(M3Model_INTERFACE)
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)

class M3AppEventModelProp_Callback : public M3AppEvent_Callback_INTERFACE {
public:

	typedef std::function<void(const M3Model_INTERFACE_SharedPtr&, const M3KVProperty_INTERFACE_SharedPtr&)> Callback_t;

protected:

	Callback_t Callback;

public:

	M3AppEventModelProp_Callback(const Callback_t& Callback) {
		this->Callback = Callback;
	};

	virtual ~M3AppEventModelProp_Callback() = default;

	void Call(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
		Callback(Model, Prop);
	}
};

class M3AppEventModelProp : public M3AppEvent_INTERFACE {

public:

	M3AppEventModelProp(int ModelId, const std::string& PropId, const M3AppEvent_Callback_INTERFACE_SharedPtr& Callback) :
		M3AppEvent_INTERFACE(GenerateGuid(ModelId, PropId), Callback) {
	}

	virtual ~M3AppEventModelProp() = default;

	void Call(const M3Model_INTERFACE_SharedPtr& Model, const M3KVProperty_INTERFACE_SharedPtr& Prop) {
		std::static_pointer_cast<M3AppEventModelProp_Callback>(Callback)->Call(Model, Prop);
	}

	static std::string GenerateGuid(int ModelId, const std::string& PropId) {
		std::stringstream StringStream;
		StringStream << static_cast<int>(ModelId) << PropId;
		return StringStream.str();
	}
};

template<typename... ARGS>
class M3AppEvent_Callback : public M3AppEvent_Callback_INTERFACE {
public:

	typedef std::function<void(ARGS...)> Callback_t;

protected:

	Callback_t Callback;

public:

	M3AppEvent_Callback(const Callback_t& Callback) {
		this->Callback = Callback;
	};

	virtual ~M3AppEvent_Callback() = default;

	void Call(ARGS... Args) {
		Callback(Args...);
	}
};

template<typename... ARGS>
class M3AppEvent : public M3AppEvent_INTERFACE {

public:

	M3AppEvent(const std::string& Id, const M3AppEvent_Callback_INTERFACE_SharedPtr& Callback) :
		M3AppEvent_INTERFACE(Id, Callback){
	}

	void Call(ARGS... Args) {
		std::static_pointer_cast<M3AppEvent_Callback<ARGS...>>(Callback)->Call(Args...);
	}
};

namespace M3Events {
	extern std::string ON_GAME_STARTED;
	extern std::string ON_ELEMENT_SWAP_ENDED;
	extern std::string ON_ELEMENT_MATCH_ENDED;
	extern std::string ON_ELEMENT_DROP_ENDED;
	extern std::string ON_ELEMENT_SPAWN_ENDED;
}

