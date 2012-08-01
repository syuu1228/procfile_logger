#include <unistd.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "interrupts.hpp"

using namespace std;
using namespace boost::algorithm;
using namespace boost::program_options;

shared_ptr<procfile_logger> init_interrupts_logger(variables_map& vm)
{
	string output = vm["output"].as<string>();
	int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
	int nintrs = 0;
	string header;
	ifstream interrupts("/proc/interrupts");
	string line;
	getline(interrupts, line);
	while(getline(interrupts, line)) {
		vector<string> cols;
		trim_all(line);
		split(cols, line, is_any_of(" "));
		if (cols[0] == "ERR:")
			break;
		header += cols[0] + ",";
		nintrs++;
	}
	shared_ptr<procfile_logger> logger(
		new procfile_logger(
			"/proc/interrupts",
			output + "/interrupts%d.log",
			1,
			nintrs,
			1,
			ncpus,
			header,
			EMPTY_VECTOR,
			true,
			PF_DIRECTION_COL
		));
	return logger;
}

