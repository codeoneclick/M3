// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

template<unsigned int NUM>
struct M3AppEvent_Params
{
	template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
	static void Forward(std::function<void(F_ARGS... FArgs)> _Function, std::tuple<T_ARGS...> const& _TArgs, ARGS... _Args)
	{
		M3AppEvent_Params<NUM - 1>::Forward(_Function, _TArgs, std::get<NUM - 1>(_TArgs), _Args...);
	}
};

template<>
struct M3AppEvent_Params<0>
{
	template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
	static void Forward(std::function<void(F_ARGS... FArgs)> _Function, std::tuple<T_ARGS...> const&, ARGS... _Args)
	{
		_Function(_Args...);
	}
};

template<typename... F_ARGS, typename... T_ARGS>
void Forward(std::function<void(F_ARGS... FArgs)> _Function, std::tuple<T_ARGS...> const& _TArgs)
{
	M3AppEvent_Params<sizeof...(T_ARGS)>::Forward(_Function, _TArgs);
};

class M3AppEvent_Callback_INTERFACE : public std::enable_shared_from_this<M3AppEvent_Callback_INTERFACE>
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

	M3AppTask_Callback(FUCTION _Function, ARGS... _Args)
	{
		Function = _Function;
		Args = std::make_tuple(_Args...);
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

	M3AppEvent_INTERFACE(const std::string& _Id, const M3AppEvent_Callback_INTERFACE_SharedPtr& _Callback) {
		Id = _Id;
		Callback = _Callback;
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

	M3AppEventModelProp_Callback(const Callback_t& _Callback) {
		Callback = _Callback;
	};

	virtual ~M3AppEventModelProp_Callback() = default;

	void Call(const M3Model_INTERFACE_SharedPtr& _Model, const M3KVProperty_INTERFACE_SharedPtr& _Prop) {
		Callback(_Model, _Prop);
	}
};

class M3AppEventModelProp : public M3AppEvent_INTERFACE {

public:

	M3AppEventModelProp(int _ModelId, const std::string& _PropId, const M3AppEvent_Callback_INTERFACE_SharedPtr& _Callback) :
		M3AppEvent_INTERFACE(GenerateGuid(_ModelId, _PropId), _Callback) {
	}

	virtual ~M3AppEventModelProp() = default;

	void Call(const M3Model_INTERFACE_SharedPtr& _Model, const M3KVProperty_INTERFACE_SharedPtr& _Prop) {
		std::static_pointer_cast<M3AppEventModelProp_Callback>(Callback)->Call(_Model, _Prop);
	}

	static std::string GenerateGuid(int _ModelId, const std::string& _PropId) {
		std::stringstream StringStream;
		StringStream << static_cast<int>(_ModelId) << _PropId;
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

	M3AppEvent_Callback(const Callback_t& _Callback) {
		Callback = _Callback;
	};

	virtual ~M3AppEvent_Callback() = default;

	void Call(ARGS... Args) {
		Callback(Args...);
	}
};

template<typename... ARGS>
class M3AppEvent : public M3AppEvent_INTERFACE {

public:

	M3AppEvent(const std::string& _Id, const M3AppEvent_Callback_INTERFACE_SharedPtr& _Callback) :
		M3AppEvent_INTERFACE(_Id, _Callback){
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

