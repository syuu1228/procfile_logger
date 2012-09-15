#include <unistd.h>
#include <boost/format.hpp>
#include "coretemp.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

void init_coretemp_logger(variables_map& vm,
	vector<std::shared_ptr<global_stats_logger> >& loggers)
{
	string output = vm["output"].as<string>();
	int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
	for (int i = 0; i < ncpus; i++) {
		std::shared_ptr<global_stats_logger> logger(
			new global_stats_logger(
				(format("/sys/devices/platform/coretemp.%d/temp1_input") % i).str(),
				(format("%s/coretemp%d.csv") % output % i).str(),
				0,
				1,
				0,
				1,
				"temp1_input",
				EMPTY_VECTOR,
				false,
				PF_DIRECTION_ROW,
				" ",
				false
			));
		loggers.push_back(logger);
	}
}
