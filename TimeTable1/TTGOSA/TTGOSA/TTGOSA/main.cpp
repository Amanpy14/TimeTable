#include <iostream>
#include "dummy_timetable.h"
#include "initial_solver.h"
#include "simulated_annealing.h"
#include "command_options.h"
#include "timetable_parser.h"
#include <iomanip>
#include<time.h>
#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {

	srand(time(0));

	try {
		auto input_file_path = Command_options(argc, argv).get_input_file_path();
		auto output_file_path = Command_options(argc, argv).get_output_file_path();

		Timetable timetable = Timetable_parser::read_timetabling_problem_from_json(input_file_path);

		//Timetable timetable = dummy_timetable::get_small_timetable();
		//timetable.print();

		Timetable::setup_for_solving();

		solve_by_simulated_annealing(timetable);
		//timetable.print();
		//cout << std::setw(4) << timetable.get_solved_json();
		cout << "Genereted solution\n";

		Timetable_parser::write_timetabling_solution_to_json(output_file_path, timetable);
		cout << "Successfully wrote solution to file";
	}
	catch (const std::string e) {
		cerr << e << endl;

		//char c;
		//cin >> c;
		return 1;
	}

	/*char c;
	cin >> c;*/
	return 0;
}
