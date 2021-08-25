#include <iostream>
#include "resource_group.h"
#include "timetable_tags.h"

/* Constructor */
Resource_group::Resource_group(int id) {
	this->id = id;
}

/* Getters */
int Resource_group::get_id() {
	return this->id;
}

std::unordered_set<int> Resource_group::get_resources() {
	return resources;
}

void Resource_group::print() {
	std::cout << id << " -> ";
	for (auto id : resources) {
		std::cout << id << " ";
	}
	std::cout << '\n';
}

void Resource_group::add_resource(int resource_id) {
	this->resources.insert(resource_id);
}
