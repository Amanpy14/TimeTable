#pragma once
#include <unordered_set>
#include <string>
#include <memory>

class Resource_group {
private:

	// ID to uniquely idenfity each resource group
	int id;

	// Set of resource IDs
	std::unordered_set<int> resources;

public:

	Resource_group(Resource_group& resource_group) = default;

	/* Constructor */
	Resource_group(int id);

	/* Getters */
	int get_id();

	std::unordered_set<int> get_resources();

	void add_resource(int resource_id);

	/* Prints the resource_gorup */
	void print();
};

using Resource_group_uptr = std::unique_ptr<Resource_group>;