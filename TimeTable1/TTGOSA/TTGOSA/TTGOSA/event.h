#pragma once
#include <vector>
#include <utility>
#include <memory>
#include <unordered_set>

#include "resource.h"
#include "json.hpp"

/*
* An event is a meeting between some resources
*/
class Event {

private:


public:
	// The event ID which must be unique to every event
	int id;

	//	A list of preassigned resources IDs which can't be changed
	std::vector<int> preassigned_resources;

	//	A list of assigned resources IDs
	//	Every item should be a pair where first = resource, second = resource group
	std::vector<std::pair<int, int>> assigned_resources;

	//	A list of unassigned resource groups
	std::vector<int> unassigned_resources;

	//	No. of time slots required
	int required_time_slots;

	// An integer which holds the time slot where the event starts
	// This will hold the actual solution time slot
	int assigned_time_slot;


	/* Constructors */
	Event(const Event& event) = default;/*{
		this->id = event.id;
		this->preassigned_resources = event.preassigned_resources;
		this->assigned_resources = event.assigned_resources;
		this->unassigned_resources = event.unassigned_resources;
		this->required_time_slots = event.required_time_slots;
		this->assigned_time_slots = event.assigned_time_slots;
	}*/

	Event() = default;

	Event(int id, int required_time_slots);

	/* Getters and Setters */
	int get_id() const;

	/* Add a preassigned_resource id 
	* resource_id is the id of the resource
	*/
	void add_preassigned_resource(int resource_id);

	/* Add a resource group from which a resource has to be assigned 
	* resource_gorup_id is the id of the resource group
	*/
	void add_unassigned_resource(int resource_group_id);

	/* Assigns the time slot to this event */
	void assign_time_slot(int time_slot);

	/* Moves all resources from assgined resources to unassinged resources
	* Set assigned_time_slot to -1
	*/
	void unassign_resources_and_time_slot();

	/* Get all the preassigned and assigned resources for this event*/
	std::vector<int> const get_all_resources();

	/* Prints the event */
	void print(std::unordered_map<int, Resource_uptr> const& resources);

	nlohmann::json get_json();
};

//using Event_uptr = std::unique_ptr<Event>;