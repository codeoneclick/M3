// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once
 
#include "Modules/ModuleManager.h"
 
class M3Impl : public IModuleInterface
{
public:

	void StartupModule();
	void ShutdownModule();
};