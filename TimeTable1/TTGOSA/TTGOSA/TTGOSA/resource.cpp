#include <string>
#include <iostream>
#include "resource.h"
#include "timetable_tags.h"

/* Constructor */
Resource::Resource(int id) {
	this->id = id;
}

/* Setters */
void Resource::set_id(int id) {
	this->id = id;
}

/* Getters */
int Resource::get_id() {
	return this->id;
}

void Resource::print() {
	std::cout << id << '\n';
}
