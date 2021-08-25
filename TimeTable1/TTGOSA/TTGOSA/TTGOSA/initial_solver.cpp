#include <iostream>

#include "initial_solver.h"
#include "solver_utils.h"

/*	Algorithm 
	while Timetable.is_solved() == False:
		Get a random incomplete event from the timetable
		Assign it random resources for each resource group
		Get a random available timeslot
		If no timeslot available then:
			return
		Assign the event to that timeslot
		mark event as complete
*/


void get_initial_solution(Timetable& timetable) {

	while (timetable.is_solved() == false) {

		int attempts_remaining = 10;
		int time_slot;

		while (attempts_remaining--) {
			// Get a random incomplete event from the timetable
			
			int event_id = timetable.get_incomplete_events().get_random();
			Event* event = timetable.get_event(event_id);

			// Assign it random resources for each resource group
			assign_random_resources(timetable, event);

			// Get a random available timeslot
			time_slot = get_random_time_slot(timetable, event);

			if (time_slot != -1) {
				// Assign the event the timeslot
				event->assign_time_slot(time_slot);

				//std::cout << "Assigend time slot: " << time_slot << "\t to event: " << event->id << '\n';

				// Mark event as complete
				timetable.mark_event_as_complete(event->get_id());

				break;
			};

			// Take back the assigned resources
			event->unassign_resources_and_time_slot();

		}
		if (time_slot == -1) {
			//std::cout << "Initial timetable generating algorithm left some events \n";
			break;
		}

	}
	
}
