#include <unistd.h>
#include "stat.hpp"

using namespace std;
using namespace boost::program_options;

shared_ptr<procfile_logger> init_stat_logger(variables_map& vm)
{
	string output = vm["output"].as<string>();
	int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
	shared_ptr<procfile_logger> logger(
		new procfile_logger(
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
	return logger;
}
