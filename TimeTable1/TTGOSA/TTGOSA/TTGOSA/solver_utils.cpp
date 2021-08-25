#include "solver_utils.h"
#include "set_random.h"

int get_random_id(std::unordered_set<int> const& s) {
	int random_number = rand() % s.size();
	auto it = s.begin();
	for (int i = 0; i < random_number; i++) {
		++it;
	}
	return *it;
}


void assign_random_resources(Timetable& timetable, Event* event) {

	// In case of multiple resources to be assigned, 
	// it is possible to assign in a wrong way where a right way might exist
	// For that, we have to make multiple attempts. Though this case is very rare in actual use cases
	int attempts = 1000;
	while (attempts--) {
		if (attempts == 1) {
			exit(801);
			return;
		}

		bool success = true;

		for (auto resource_group : event->unassigned_resources) {
			// Get resources of this resource group
			std::unordered_set<int> resources = timetable.get_resources_from_resource_group(resource_group);

			// Get all the preassigned and assigned resources of this event
			std::vector<int> event_resources = event->get_all_resources();

			// Delete preassigned and assigned resources from resources
			for (auto resource : event_resources) {
				auto it = resources.find(resource);
				if (it != resources.end()) resources.erase(it);
			}

			if (resources.size() == 0) {
				event->assigned_resources.resize(0);
				success = false;
				break;
			}

			// Get a random resource from the remaining resources of the resource group
			int resource_id = get_random_id(resources);

			// Insert this resource in assigned resource of the event
			event->assigned_resources.push_back({ resource_id, resource_group });
		}
		if (success) break;
	}

	// Delete all from unassigned resources
	event->unassigned_resources.resize(0);
}


int get_random_time_slot(Timetable& timetable, Event* event) {

	Set_random available_time_slots;

	// Iterate through all the time slots and put them in a set
	for (int i = 0; i < timetable.get_time_slots_count(); i++) {
		if (timetable.is_time_slot_valid_for_event(i, event)) {
			available_time_slots.insert(i);
		}
	}

	if (available_time_slots.size() == 0) return -1;
	return available_time_slots.get_random();
}
