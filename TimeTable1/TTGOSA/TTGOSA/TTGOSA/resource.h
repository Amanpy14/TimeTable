#pragma once

#include <string>
#include <unordered_set>
#include <memory>

/* One object of Resource class represents one resource
* A resource can be any of a section, teacher, or room
*/
class Resource {
private:

	// An id to uniquely identify each resource
	int id;

	// A set containing the IDs of all the resource groups which this resource is part of
	std::unordered_set<int> resource_groups;

public:

	Resource(Resource& resource) = default;

	/* Constructor */
	Resource(int id);

	/* Setters */
	void set_id(int id);

	/* Getters */
	int get_id();

	/* Prints the resource to the console */
	void print();

};

using Resource_uptr = std::unique_ptr<Resource>;