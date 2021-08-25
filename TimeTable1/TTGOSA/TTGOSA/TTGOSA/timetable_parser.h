#pragma once

#include "timetable.h"
#include "json.hpp"

class Timetable_parser {
private:
	static nlohmann::json get_json_problem(std::string json_file_path);
	static Timetable create_timetable_problem(nlohmann::json& json_problem);
	static void fill_resources(nlohmann::json& json_resources, Timetable& timetable);
	static void fill_resource_groups(nlohmann::json& json_resource_groups, Timetable& timetable);
	static void fill_conflicting_resources(nlohmann::json& json_conflicting_resources, Timetable& timetable);
	static void fill_events(nlohmann::json& json_events, Timetable& timetable);
	static void fill_no_resource_availability(nlohmann::json& json_busy_slots, Timetable& timetable);
	static void fill_specific_resource_availability(nlohmann::json& json_resource_busy_slots, Timetable& timetable);
public:
	static Timetable read_timetabling_problem_from_json(std::string json_file_path);
	static void write_timetabling_solution_to_json(std::string json_file_path, Timetable& timetable);
};
