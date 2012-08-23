#include <unistd.h>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "interrupts.hpp"

using namespace std;
using namespace boost::algorithm;
using namespace boost::program_options;

void init_interrupts_logger(variables_map& vm,
	vector<shared_ptr<global_stats_logger> >& loggers)
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
	shared_ptr<global_stats_logger> logger(
		new global_stats_logger(
			"/proc/interrupts",
			output + "/interrupts%d.csv",
			1,
			nintrs,
			1,
			ncpus,
			header,
			EMPTY_VECTOR,
			true,
			PF_DIRECTION_COL
		));
	loggers.push_back(logger);
}

