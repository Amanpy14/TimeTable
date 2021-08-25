
#include "command_options.h"

Command_options::Command_options(int argc, char* argv[]) {

	op::OptionParser opt;
	opt.add_option("h", "help", "shows option help");
	opt.add_option("i", "input_file_path", "input file", "");
	opt.add_option("o", "output_file_path", "output file", "");

	if (!opt.parse_options(argc, argv)) {
		throw std::string("Incorrect options");
	}

	input_file_path = opt["i"];
	output_file_path = opt["o"];
	if (op::str2bool(opt["h"])) {
		opt.show_help();
	}
}

std::string Command_options::get_input_file_path() {

	return input_file_path;
}

std::string Command_options::get_output_file_path() {

	return output_file_path;
}
