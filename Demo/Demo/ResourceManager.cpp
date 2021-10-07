#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ResourceManager::AddResource(std::string resource_name, int resource_amount)
{
	//If the resource already exists in the characters inventory.
	if (resources.find(resource_name) != resources.end()) {
		int new_amount = resources[resource_name] + resource_amount;
		if (new_amount >= 0) {
			resources[resource_name] = new_amount;
			return true;
		}
	}
	//If the resource didnt already exist in the characters inventory.
	else {
		resources.insert(std::pair<std::string, int>(resource_name, 0));
		if (resource_amount >= 0) {
			resources[resource_name] = resource_amount;
			return true;
		}
	}
	return false;
}

bool ResourceManager::CanAddResource(std::string resource_name, int resource_amount)
{
	//If the resource already exists then see if the amount can be added.
	if (resources.find(resource_name) != resources.end()) {
		int new_amount = resources[resource_name] + resource_amount;
		if (new_amount >= 0) {
			return true;
		}
	}
	//If the resource didnt already exist in the characters inventory.
	else {
		resources.insert(std::pair<std::string, int>(resource_name, 0));
	}
	return false;
}

void ResourceManager::LoadInitialResourcesFromFile(LocationManager* location_manager)
{
	std::fstream file;
	file.open("OtherFiles/ResourceData.csv");
	std::string line, word, temp;

	while (file >> temp) {
		std::vector<std::string> row;

		std::getline(file, line);
		temp += line;

		std::stringstream s(temp);

		while (std::getline(s, word, ',')) {
			row.push_back(word);
		}

		if (row.size() > 0) {
			AddResource(row[0], 0);
			std::vector<std::string> resource_names;
			std::vector<int> resource_amounts;
			int i = 2;
			while (i < row.size() && row[i] != "") {
				resource_names.push_back(row[i]);
				resource_amounts.push_back(std::stoi(row[i + 1]));
				i += 2;
			}
			location_manager->AddTaskToLocations(row[1], row[0],
				resource_names, resource_amounts);
		}
	}
}
