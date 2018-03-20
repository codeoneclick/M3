// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once
 
#include "ModuleManager.h"
 
class M3Impl : public IModuleInterface
{
public:

	void StartupModule();
	void ShutdownModule();
};