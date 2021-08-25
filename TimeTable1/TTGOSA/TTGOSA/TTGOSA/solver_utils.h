#pragma once
#include "timetable.h"

/* Returns a random id from a set of integers */
int get_random_id(std::unordered_set<int> const& s);


/* For all unassigned resource gorups, select a random resource and assign it
*/
void assign_random_resources(Timetable& timetable, Event* event);


/* Returns an appropriate time slot for the given event in the given timetable
* If no timeslot available then returns -1
*/
int get_random_time_slot(Timetable& timetable, Event* event);