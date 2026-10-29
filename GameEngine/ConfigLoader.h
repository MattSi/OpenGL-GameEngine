#pragma once
//#include "StandardIncludes.h"
#include "config.h"

class ConfigLoader
{
public:
	static AppConfig LoadAppConfig(const std::string& filename);
};

