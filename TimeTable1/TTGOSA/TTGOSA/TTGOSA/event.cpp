#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "timetable.h"
#include "event.h"
#include "resource.h"
#include "timetable_tags.h"

Event::Event(int id, int required_time_slots) {
	this->id = id;
	this->required_time_slots = required_time_slots;
	this->assigned_time_slot = -1;
}

int Event::get_id()const {
	return id;
}

void Event::add_preassigned_resource(int resource_id) {
	preassigned_resources.push_back(resource_id);
}

void Event::add_unassigned_resource(int resource_group_id) {
	unassigned_resources.push_back(resource_group_id);
}

void Event::assign_time_slot(int time_slot) {
	assigned_time_slot = time_slot;
}

void Event::unassign_resources_and_time_slot()
{	
	// Move assigned resources to unassigned resources
	for (auto& assigned_resource : assigned_resources) {
		int resource_group = assigned_resource.second;
		unassigned_resources.push_back(resource_group);
	}
	assigned_resources.resize(0);

	// Remove assigned time slot
	assigned_time_slot = -1;
}

std::vector<int> const Event::get_all_resources() {

	std::vector<int> resources;

	// get preassigned resources and assigned resources
	for (auto& resource : preassigned_resources) {
		resources.push_back(resource);
	}
	for (auto& resource_group : assigned_resources) {
		resources.push_back(resource_group.first);
	}

	return resources;
}

void Event::print(std::unordered_map<int, Resource_uptr>const& resources) {
	std::cout << id << ". Required time slots = " << required_time_slots;
	std::cout << ", Assigned time slot = " << assigned_time_slot << '\n';
	std::cout << "Preassigned resources: ";
	for (auto id : preassigned_resources) {
		std::cout << resources.find(id)->second->get_id() << ", ";
	}
	std::cout << "\nAssigned resources: ";
	for (auto id : assigned_resources) {
		std::cout << resources.find(id.first)->second->get_id() << " " << id.second << ", ";
	}
	std::cout << "\nUnassigned resource groups: ";
	for (auto id : unassigned_resources) {
		std::cout << id << " ";
	}
	std::cout << '\n';
}


nlohmann::json Event::get_json() {
	using nlohmann::json;
	json event_json;
	event_json[timetable_tags::events_tags::id] = id;
	event_json[timetable_tags::events_tags::required_time_slots] = required_time_slots;
	event_json[timetable_tags::events_tags::assigned_time_slot] = Timetable::get_day_and_period(assigned_time_slot);
	event_json[timetable_tags::events_tags::preassigned_resources] = preassigned_resources;
	event_json[timetable_tags::events_tags::assigned_resources] = assigned_resources;
	event_json[timetable_tags::events_tags::unassigned_resources] = unassigned_resources;
	
	return event_json;
}
