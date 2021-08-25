#include "simulated_annealing.h"
#include "initial_solver.h"
#include "solver_utils.h"
#include <iostream>


Timetable get_neighbour(Timetable& const timetable, int entropy);
int get_weight(Timetable& const timetable);
void clear_random_continuous_time_slots(int continuous_time_slots_required, Timetable& neighbour);
int get_continuous_time_slots_required_for_incomplete_events(Timetable& neighbour);



void solve_by_simulated_annealing(Timetable& timetable)
{
	int total_iterations = 10000;
	int iterations = total_iterations;

	Timetable current_timetable = timetable;
	Timetable best_timetable = timetable;
	int best_timetable_weight = get_weight(best_timetable);
	int total_events = timetable.get_incomplete_events().size();

	int entropy = 2, max_entropy = 2;
	while (iterations--) {

		Timetable neighbour = get_neighbour(current_timetable, entropy);
		int neighbour_weight = get_weight(neighbour);
		int current_timetable_weight = get_weight(current_timetable);

		entropy = std::max(rand() % (max_entropy + 1), 2);
		//entropy = 12;
		std::cout << "Iterations remaining: " << iterations << "\t neighbour: " << neighbour_weight << "\t current: " << current_timetable_weight << "\t best: " << best_timetable_weight << '\n';
		std::cout << "Entropy: " << entropy << '\n';

		if (neighbour_weight <= current_timetable_weight) {

			current_timetable = neighbour;

			if (neighbour_weight < best_timetable_weight) {
				max_entropy = 2;
				best_timetable = current_timetable;
				best_timetable_weight = neighbour_weight;
			}

			if (neighbour_weight == 0) break;
		}
		else {
			//if (rand() % total_iterations < iterations/2) current_timetable = neighbour;
			//entropy = std::min(entropy + 2, std::max(8, total_events/5));
			if (rand() % 5 == 0) max_entropy = std::min(max_entropy + 1, total_events/5);

		}
	/*	entropy = total_events % (rand() % (total_events / 2) + 1);
		entropy = std::max(entropy, 2);*/
		//std::cout << "Weight: " << get_weight(neighbour) << 
	}

	timetable = best_timetable;
}


/* Returns a neighbour timetable 
   Entropy: Higher value means more disruption is allowed to generate neighbour
			Higher value allows distant neighbours to be found but takes more time
*/
Timetable get_neighbour(Timetable& const timetable, int entropy) 
{
	Timetable neighbour = timetable;
	
	// Remove as many completed events as entropy
	while (entropy--) {

		auto& complete_events = neighbour.get_complete_events();
		// If there are no complete events to remove then break out
		if (complete_events.size() == 0) break;

		int random_event = complete_events.get_random();
		neighbour.mark_event_as_incomplete(random_event);

	}

	int continuous_time_slots_required = get_continuous_time_slots_required_for_incomplete_events(neighbour);

	for (int i=0; i < std::max(1, entropy/10); i++)
		clear_random_continuous_time_slots(continuous_time_slots_required, neighbour);

	get_initial_solution(neighbour);

	return neighbour;
}

int get_continuous_time_slots_required_for_incomplete_events(Timetable& neighbour) {

	int continuous_time_slots_required = 0;

	for (auto event_id : neighbour.get_incomplete_events().get_values()) {
		// Get required time slots of selected random event
		int required_time_slots = neighbour.get_event(event_id)->required_time_slots;
		continuous_time_slots_required = std::max(continuous_time_slots_required, required_time_slots);
	}

	return continuous_time_slots_required;
}

void clear_random_continuous_time_slots(int continuous_time_slots_required, Timetable& neighbour) {
	int random_time_slot = rand() % neighbour.get_time_slots_count();

	for (int i = random_time_slot; i < random_time_slot + continuous_time_slots_required && i < neighbour.get_time_slots_count(); i++) {
		for (auto event_id : neighbour.get_all_events_starting_at_time_slot(i)) {
			neighbour.mark_event_as_incomplete(event_id);
		}
	}
}

int get_weight(Timetable& const timetable) {
	int weight = 0;

	auto& incomplete_events = timetable.get_incomplete_events();
	weight = incomplete_events.size();

	return weight;
}
