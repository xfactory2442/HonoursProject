#pragma once
#include <unordered_map>
#include "LocationManager.h"

class ResourceManager
{
public:
	static bool AddResource(std::string resource_name, int resource_amount);
	static bool CanAddResource(std::string resource_name, int resource_amount);
	static void LoadInitialResourcesFromFile(LocationManager* location_manager);
	static std::unordered_map<std::string, unsigned int> resources;
};

