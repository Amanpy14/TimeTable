#pragma once
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include "set_random.h"

#include "event.h"
#include "resource.h"
#include "resource_group.h"
#include "json.hpp"

/*
* This class is responsible to hold all the information about a timetable including all the resources involved and the time slots. 
* It will also incorporate a resource being busy at a particualr time slot constraint.
*/
class Timetable {

private:

	// A map storing all the events, indexed by the event ID
	std::unordered_map<int, Event> all_events;

	//	A set of ID's of incomplete events
	Set_random incomplete_events;
	
	//	A set of ID's of complete events
	Set_random complete_events;


	//	A non static list of time slots where, each element is a set of resource IDs
	//	For incorporating timetable specific constraints
	//	For example, when an event is assigned a timeslot, we need to keep track
	//	that the resources involded in that event are now busy at that timeslot
	std::vector< std::unordered_set<int> > time_slot_to_resources_map;

	// To get all events that are assigned a paritucular time slot efficiently
	std::vector< std::unordered_set<int> > time_slot_to_events_map;


public:

	static int days; // Number of days in a timetable cycle
	static int time_slots_per_day; // Number of maximum timeslots in a day

	//	A static list of time slots
	//	Each element is a set of resources which are not available at that time slot
	//	For incorporating global constraints like lunch and half days
	static std::vector< std::unordered_set<int> > global_time_slots;

	// A static list of all resources
	static std::unordered_map<int, Resource_uptr> resources;

	//	A static list defining all the conflict resources
	//	Indexed by the resource ID, value is a set of resource IDs which are the conflict resources for the i’th resource
	static std::unordered_map<int, std::unordered_set< int > > conflict_resources;

	//	A static list defining all the resource gorups
	//	Indexed by the resource group ID, vlaue is a set of IDs of those resources which are contained in this group
	static std::unordered_map< int, Resource_group_uptr > resource_groups;

	/* Get time_slot from day and period */
	static int get_time_slot(int day, int period);

	/* Get day and period from time_slot */
	static std::vector<int> get_day_and_period(int time_slot);

	/* Default Constructor */
	Timetable();
	//~Timetable()
	//{
	//	std::cout << "Destructing" << std::endl;
	//}
	/* An initializing function for setting some global information
	* days: The number of days in a timetable cycle
	* time_slots_per_day: Number of time_slots in a single day
	*/
	static void set_time_slots(int days, int time_slots_per_day);

	/* Lets the user add a new resource */
	static void add_resource(Resource& resource);

	/* Lets the user add a new resource group */
	static void add_resource_group(Resource_group& resource_group);

	/* Lets the user add a pair of conflicting resources */
	static void add_conflict_resource_pair(int resource_id1, int resource_id2);

	/* Processes the input for solving. Must be called exactly once before starting to solve */
	static void setup_for_solving();

	/* Lets the user add an event */
	void add_event(Event& event);

	/* Makes all the resources busy for the given timeslot */
	static void make_all_resources_busy(int day, int period);

	/* Makes a particular resource busy at the given timeslot */
	static void make_resource_busy(int day, int period, int resource_id);
	
	/* Print the current state of the timetable */
	void print();

	/* Reaturns True if the the timetable is solved */
	bool is_solved();

	/* Returns a referance to incomplete events */
	Set_random& get_incomplete_events();

	/* Returns a referance to complete events */
	Set_random& get_complete_events();

	/* Returns the pointer to the Event object with the given ID */
	Event* get_event(int id);
	
	/* Returns if the given event can be scheduled the given timeslot wihout violating any hard constraints */
	bool is_time_slot_valid_for_event(int time_slot, Event* event);

	/* Returns the total timeslots count */
	int get_time_slots_count();

	/* Makrs the event as complete 
	* The event must have no unassigned resources or time slots	
	*/
	void mark_event_as_complete(int event_id);

	/* Marks the event as incomplete
	* The event must have some unassigned resources or time slots
	*/
	void mark_event_as_incomplete(int event_id);

	/* Get resources of a resource group */
	std::unordered_set<int> get_resources_from_resource_group(int resource_group_id);

	/* Gett all events starting at a particular time slot */
	std::vector<int> get_all_events_starting_at_time_slot(int time_slot);

	/* Get conflicting resources for a list of resources */
	std::vector<int> get_conflicting_resources(std::vector<int> const& resources);

	nlohmann::json get_solved_json();

};
