#include <fstream>
#include <stdio.h>
#include <iostream>

#include "dummy_timetable.h"
#include "timetable.h"

void add_resources(Timetable& timetable, std::string path);
void add_resource_groups(Timetable& timetable, std::string path);
void add_conflicting_resources(Timetable& timetable, std::string path);
void add_events(Timetable& timetable, std::string path);

Timetable dummy_timetable::get_small_timetable() {
	
	// Set time slots
	Timetable::set_time_slots(6, 8);

	// Create timetable object
	Timetable timetable;


	// Path of file
	std::string path = "..//test_cases//small_test_case_1//";

	// Add resources
	add_resources(timetable, path);

	// Add resource groups
	add_resource_groups(timetable, path);

	// Add events
	add_events(timetable, path);

	// Add conflicting resources
	add_conflicting_resources(timetable, path);

	

	// Add lunch breaks and different timings of sections at different days
	// Add 5th timeslot of each day as busy for all
	for (int i = 1; i <= 6; i++) {
		timetable.make_all_resources_busy(i, 5);
	}
	// Make wednesday and saturday half days
	for (int i = 6; i <= 8; i++) {
		timetable.make_all_resources_busy(3, i); // wednesday
		timetable.make_all_resources_busy(6, i); // saturday
	}

	// Add time slots for already done events

	return timetable;
}


void add_resources(Timetable& timetable, std::string path) {

	// Open file
	std::string complete_path = path + "resources.txt";
	freopen(complete_path.c_str(), "r", stdin);

	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		int id;
		std::string type, code;
		std::cin >> id >> type >> code;
		Resource resource(id);
		timetable.add_resource(resource);
	}
	std::cout << "Done adding resources\n";
	fclose(stdin);
}


void add_resource_groups(Timetable& timetable, std::string path) {
	// Open file
	std::string complete_path = path + "resource_groups.txt";
	freopen(complete_path.c_str(), "r", stdin);

	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		int id;
		std::string name;
		std::cin >> id >> name;
		Resource_group resource_group(id);

		int m;
		std::cin >> m;
		while (m--) {
			int resource_id;
			std::cin >> resource_id;
			resource_group.add_resource(resource_id);
		}
		timetable.add_resource_group(resource_group);
	}
	std::cout << "Done adding resource_groups\n";
	fclose(stdin);

}


void add_conflicting_resources(Timetable& timetable, std::string path) {
	
	// Open file
	std::string complete_path = path + "conflicting_resources.txt";

	std::fstream file;
	file.open(complete_path);

	int n;
	file >> n;
	while (n--) {
		int id1, id2;
		file >> id1 >> id2;
		timetable.add_conflict_resource_pair(id1, id2);
	}
	std::cout << "Done adding conflict pairs\n";
	file.close();

}


void add_events(Timetable& timetable, std::string path) {
	// Open file
	std::string complete_path = path + "events.txt";

	std::fstream file;
	file.open(complete_path);

	int event_id = 0, resource_id;
	while (true) {
		int required_timeslots, preassigned_count, unassigned_count;
		file >> required_timeslots >> preassigned_count >> unassigned_count;

		if (required_timeslots == -1) break;

		Event event(event_id++, required_timeslots);
		while (preassigned_count--) {
			file >> resource_id;
			event.add_preassigned_resource(resource_id);
		}
		while (unassigned_count--) {
			file >> resource_id;
			event.add_unassigned_resource(resource_id);
		}
		timetable.add_event(event);
	}

	std::cout << "Done adding events. Events count = " << event_id << '\n';
	
}