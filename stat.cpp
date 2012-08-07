#include <unistd.h>
#include "stat.hpp"

using namespace std;
using namespace boost::program_options;

void init_stat_logger(variables_map& vm,
	vector<shared_ptr<global_stats_logger> >& loggers)
{
	string output = vm["output"].as<string>();
	int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
	shared_ptr<global_stats_logger> logger(
		new global_stats_logger(
			"/proc/stat",
			output + "/stat%d.log",
			1,
			ncpus,
			1,
			10,
			"user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice",
			EMPTY_VECTOR,
			true
		));
	loggers.push_back(logger);
}
