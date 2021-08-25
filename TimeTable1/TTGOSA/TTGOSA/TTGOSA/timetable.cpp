#include <iostream>

#include "timetable.h"
#include "resource_group.h"
#include "timetable_tags.h"

int Timetable::days;
int Timetable::time_slots_per_day;
std::vector< std::unordered_set<int> > Timetable::global_time_slots;
std::unordered_map<int, Resource_uptr> Timetable::resources;
std::unordered_map<int, std::unordered_set< int > > Timetable::conflict_resources;
std::unordered_map< int, Resource_group_uptr > Timetable::resource_groups;

int Timetable::get_time_slot(int day, int period) {
	int time_slot_index = (day - 1) * (time_slots_per_day + 1) + period;
	time_slot_index--; // as time_slots are 0 indexed
	return time_slot_index;
}

std::vector<int> Timetable::get_day_and_period(int time_slot) {
	std::vector<int> day_and_period;
	day_and_period.push_back(time_slot / (time_slots_per_day + 1) + 1);
	day_and_period.push_back((time_slot + 1) % (time_slots_per_day + 1));
	return day_and_period;
}

Timetable::Timetable() {
	// Resize the time_slot vector
	std::cout << Timetable::days * (Timetable::time_slots_per_day + 1) << '\n';
	time_slot_to_resources_map.resize(Timetable::days * (Timetable::time_slots_per_day + 1));
	time_slot_to_events_map.resize(Timetable::days * (Timetable::time_slots_per_day + 1));
}

void Timetable::set_time_slots(int days, int time_slots_per_day) {

	// Set days and time_slots_per_day
	Timetable::days = days;
	Timetable::time_slots_per_day = time_slots_per_day;

	// Resize time_slot vectors
	// Added 1 in time_slots_per_day for the terminal slot at the end of each day
	Timetable::global_time_slots.resize(days * (time_slots_per_day + 1));

}

void Timetable::add_resource(Resource& resource) {
	resources[resource.get_id()] = std::make_unique<Resource>(resource);
}

void Timetable::add_resource_group(Resource_group& resource_group) {
	resource_groups[resource_group.get_id()] = std::make_unique<Resource_group>(resource_group);
}

void Timetable::add_event(Event& event) {

	// Insert in the all_events map
	all_events[event.get_id()] = event;

	// Insert the id in incomplete event set
	incomplete_events.insert(event.get_id());
}

void Timetable::make_all_resources_busy(int day, int period) {

	
	int time_slot = get_time_slot(day, period);

	// Add all the resources to this time_slot
	for (auto &resource : resources) {
		global_time_slots[time_slot].insert(resource.first);
	}
}

void Timetable::make_resource_busy(int day, int period, int resource_id) {

	int time_slot = get_time_slot(day, period); 

	global_time_slots[time_slot].insert(resource_id);
}

void Timetable::print() {
	std::cout << "Resources\n";
	for (int i = 1; i <= resources.size(); i++) {
		//std::cout << i << '\n';
		resources[i]->print();
	}
	std::cout << "\n\nResource Groups \n";
	for (int i = 1; i <= resource_groups.size(); i++) {
		resource_groups[i]->print();
	}
	std::cout << "\n\nEvents \n";
	//for (int i = 0; i < all_events.size(); i++) {
	//	all_events[i].print();
	//}
	for (auto & event : all_events) {
		event.second.print(resources);
		std::cout << '\n';
	}
	std::cout << "\n--------------------------------------------------------------\n";
}

bool Timetable::is_solved() {
	return incomplete_events.size() == 0;
}

Set_random& Timetable::get_incomplete_events() {
	return incomplete_events;
}

Set_random& Timetable::get_complete_events()
{
	return complete_events;
}

Event* Timetable::get_event(int id) {
	Event *pointer = &all_events[id];
	return pointer;
}

bool Timetable::is_time_slot_valid_for_event(int time_slot, Event* event) {
	/*std::cout << "time slot: " << time_slot << "\n ";
	event->print(resources);*/

	std::vector<int> resources = event->get_all_resources();

	// Append conflicting resources to this vector
	std::vector<int> conflicting_resources = Timetable::get_conflicting_resources(resources);
	resources.insert(resources.end(), conflicting_resources.begin(), conflicting_resources.end());

	// None of the resource must already exist at this time slot and required_time_slots number of slots after this time slot
	for (auto resource = resources.begin(); resource != resources.end(); ++ resource) {
		for (int i = 0; i < event->required_time_slots; i++) {
			if (time_slot + i >= global_time_slots.size())
				return false;

			if (global_time_slots[time_slot + i].find(*resource) != global_time_slots[time_slot + i].end()
				|| time_slot_to_resources_map[time_slot + i].find(*resource) != time_slot_to_resources_map[time_slot + i].end()) {
				return false;
			}
		}
	}
	
	return true;
}

int Timetable::get_time_slots_count() {
	return global_time_slots.size();
}

void Timetable::mark_event_as_complete(int event_id) {
	incomplete_events.erase(event_id);
	complete_events.insert(event_id);

	Event event = all_events[event_id]; // Get event

	// Add event to the starting time slot in time_slot_to_event_map
	time_slot_to_events_map[event.assigned_time_slot].insert(event_id);

	// Add resources 
	for (auto resource : event.get_all_resources()) {
		for (int i = event.assigned_time_slot; i < event.assigned_time_slot + event.required_time_slots; i++) {
			time_slot_to_resources_map[i].insert(resource);
		}
	}
}

void Timetable::mark_event_as_incomplete(int event_id) {
	complete_events.erase(event_id);
	incomplete_events.insert(event_id);


	Event event = all_events[event_id]; // Get event

	// Remove event from the starting time slot in time_slot_to_events_map
	time_slot_to_events_map[event.assigned_time_slot].erase(event_id);

	// Remove resources from time_sltos
	for (auto resource : event.get_all_resources()) {
		for (int i = event.assigned_time_slot; i < event.assigned_time_slot + event.required_time_slots; i++) {
			time_slot_to_resources_map[i].erase(resource);
		}
	}
}

std::unordered_set<int> Timetable::get_resources_from_resource_group(int resource_group_id) {
	return resource_groups[resource_group_id]->get_resources();
}

std::vector<int> Timetable::get_all_events_starting_at_time_slot(int time_slot)
{
	std::vector<int> events;
	for (auto event_id : time_slot_to_events_map[time_slot]) {
		events.push_back(event_id);
	}
	return events;
}

std::vector<int> Timetable::get_conflicting_resources(std::vector<int> const& resources) {
	std::unordered_set<int> conflict_resource_set;
	
	// Get the conflict resources in a set
	for (auto resource : resources) {
		for (auto conflict_resource : Timetable::conflict_resources[resource]) {
			conflict_resource_set.insert(conflict_resource);
		}
	}

	// Make the vector from the set
	std::vector<int> conflict_resources;
	for (auto resource : conflict_resource_set) {
		conflict_resources.push_back(resource);
	}

	return conflict_resources;
}


void Timetable::add_conflict_resource_pair(int resource_id1, int resource_id2) {

	// Add each other as conflict resources
	conflict_resources[resource_id1].insert(resource_id2);
	conflict_resources[resource_id2].insert(resource_id1);
}

void Timetable::setup_for_solving()
{
	/* Make all resources busy at the last slot of each day */
	/* This last slot was added only for solution purpose */
	for (int i = 1; i <= days; i++) {
		make_all_resources_busy(i, time_slots_per_day + 1);
	}
}

nlohmann::json Timetable::get_solved_json() {
	using nlohmann::json;
	json solved_json;

	for (auto& event : all_events) {
		solved_json.emplace_back(event.second.get_json());
	}

	return solved_json;
}
