#pragma once

#include "optionParser.hpp"

class Command_options {
private:
	std::string input_file_path;
	std::string output_file_path;

public:
	Command_options(int argc, char* argv[]);
	std::string get_input_file_path();
	std::string get_output_file_path();
};