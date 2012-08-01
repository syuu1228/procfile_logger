#include <unistd.h>
#include <string>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

using namespace std;
using namespace boost::program_options;

int main(int argc, char **argv)
{
	options_description opt("options");
	opt.add_options()
		("help,h", "show help")
		("output,o",
			value<string>()->default_value("."), "output dir")
		("duration,d", 
			value<int>()->default_value(1), "duration sec")
		("terminate,t", 
			value<int>()->default_value(60), "terminate sec");
	variables_map vm;
	store(parse_command_line(argc, argv, opt), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << opt << endl;
		return 1;
	}else{
		int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
		procfile_logger logger(
			"/proc/stat",
			vm["output"].as<string>() + "/stat%d.log",
			1,
			ncpus,
			1,
			10,
			"user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice",
			true
		);
		for (int i = 0; i <= vm["terminate"].as<int>(); i++) {
			logger.update();
			if (i == 0)
			logger.start();
			sleep(vm["duration"].as<int>());
		}
	}

	return 0;
}
