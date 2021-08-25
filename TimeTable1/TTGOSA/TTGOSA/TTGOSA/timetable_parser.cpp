#include "timetable_parser.h"
#include "timetable_tags.h"
#include "resource.h"
#include "resource_group.h"
#include "event.h"
#include "json.hpp"
#include <fstream>

using namespace std;
using nlohmann::json;

json Timetable_parser::get_json_problem(std::string json_file_path) {
	ifstream file_stream(json_file_path);
	if (file_stream) {
		json json_problem;
		file_stream >> json_problem;
		file_stream.close();
		return move(json_problem);
	}
	throw string("File cannot be opened " + json_file_path);
}

Timetable Timetable_parser::create_timetable_problem(nlohmann::json& json_problem) {
	Timetable::set_time_slots(
		json_problem[timetable_tags::days], 
		json_problem[timetable_tags::slots_per_day]);
	Timetable timetable_problem;

	fill_resources(json_problem[timetable_tags::resources], timetable_problem);
	fill_resource_groups(json_problem[timetable_tags::resource_groups], timetable_problem);
	fill_conflicting_resources(json_problem[timetable_tags::conflicting_resources], timetable_problem);
	fill_events(json_problem[timetable_tags::events], timetable_problem);
	fill_no_resource_availability(json_problem[timetable_tags::all_resources_busy], timetable_problem);
	fill_specific_resource_availability(json_problem[timetable_tags::resources_busy], timetable_problem);

	return timetable_problem;
}

void Timetable_parser::fill_resources(nlohmann::json& json_resources, Timetable& timetable)
{
	for (auto json_resource : json_resources) {
		//Resource resource(json_resource[timetable_tags::id]);
		Resource resource(json_resource);
		timetable.add_resource(resource);
	}
}

void Timetable_parser::fill_resource_groups(nlohmann::json& json_resource_groups, Timetable& timetable)
{
	for (auto json_resource_group : json_resource_groups) {
		Resource_group resource_group(json_resource_group[timetable_tags::resource_groups_tags::id]);
		
		for (auto json_resourceId : 
			json_resource_group[timetable_tags::resource_groups_tags::resourceIds]) {

			resource_group.add_resource(json_resourceId);
		}
		timetable.add_resource_group(resource_group);
	}
}

void Timetable_parser::fill_conflicting_resources(nlohmann::json& json_conflicting_resources, Timetable& timetable) {
	for (auto json_conflicting_resource_pair : json_conflicting_resources) {
		timetable.add_conflict_resource_pair(
			json_conflicting_resource_pair[0],
			json_conflicting_resource_pair[1]);
	}
}

void Timetable_parser::fill_events(nlohmann::json& json_events, Timetable& timetable) {
	for (auto json_event : json_events) {
		Event event(
			json_event[timetable_tags::events_tags::id],
			json_event[timetable_tags::events_tags::required_time_slots]);

		for (auto json_preassigned_resource : 
			json_event[timetable_tags::events_tags::preassigned_resources]) {

			for (auto resource_id : json_preassigned_resource) {
				event.add_preassigned_resource(resource_id);
			}
		}
		for (auto json_uneassigned_resource :
			json_event[timetable_tags::events_tags::unassigned_resources]) {

			for (auto resource_group_id : json_uneassigned_resource) {
				event.add_unassigned_resource(resource_group_id);
			}
		}

		timetable.add_event(event);
	}
}

void Timetable_parser::fill_no_resource_availability(nlohmann::json& json_busy_slots, Timetable& timetable) {
	for (auto json_busy_slot_details : json_busy_slots) {
		for (auto day : json_busy_slot_details[timetable_tags::busy_resources_tags::days]) {
			for (auto time_slot : json_busy_slot_details[timetable_tags::busy_resources_tags::time_slots]) {
				timetable.make_all_resources_busy(day, time_slot);
			}
		}
	}
}

void Timetable_parser::fill_specific_resource_availability(nlohmann::json& json_resource_busy_slots, Timetable& timetable) {
	for (auto json_resource_busy_slot_details : json_resource_busy_slots) {
		for (auto day : json_resource_busy_slot_details[timetable_tags::busy_resources_tags::days]) {
			for (auto time_slot : json_resource_busy_slot_details[timetable_tags::busy_resources_tags::time_slots]) {
				timetable.make_resource_busy(day, time_slot,
					json_resource_busy_slot_details[timetable_tags::busy_resources_tags::resource_id]);
			}
		}
	}
}

Timetable Timetable_parser::read_timetabling_problem_from_json(std::string json_file_path) {
	auto json_problem = get_json_problem(json_file_path);
	
	auto l1 = json_problem["days"];
	auto l2 = json_problem["resources"];
	auto l3 = json_problem["events"];
	
	return create_timetable_problem(json_problem);
}

void Timetable_parser::write_timetabling_solution_to_json(std::string json_file_path, Timetable& timetable) {
	ofstream file_stream(json_file_path);
	if (file_stream) {
		file_stream << timetable.get_solved_json();
		file_stream.close();
		return;
	}
	throw string("File cannot be opened " + json_file_path);
}
