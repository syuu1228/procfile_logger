#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include "timer_runner.hpp"
#include "interrupts.hpp"
#include "netstat.hpp"
#include "stat.hpp"
#include "schedstat.hpp"

using namespace std;
using namespace boost::program_options;

int main(int argc, char **argv)
{
	options_description opt("options");
	variables_map vm;

	opt.add_options()
		("help,h", "show help")
		("target,t",
		value<vector<string> >()->composing(),
		"target procfiles(interrupts,netstat,stat,schedstat)")
		("output,o",
		value<string>()->default_value("."), "output dir")
		("duration,d", 
		value<int>()->default_value(1), "duration sec")
		("terminate,T", 
		value<int>()->default_value(0), "terminate sec");
	store(parse_command_line(argc, argv, opt), vm);
	notify(vm);

	if (vm.count("help") || !vm.count("target")) {
		cout << opt << endl;
		return 1;
	}else{
		vector<string> targets = vm["target"].as<vector<string> >();
		vector<shared_ptr<global_stats_logger> > loggers;
		for (auto t : targets) {
			if (t == "interrupts") {
				init_interrupts_logger(vm, loggers);
			}else if(t == "netstat") {
				init_netstat_logger(vm, loggers);
			}else if(t == "stat") {
				init_stat_logger(vm, loggers);
			}else if(t == "schedstat") {
				init_schedstat_logger(vm, loggers);
			}else{
				cout << opt << endl;
				return 1;
			}
		}
		timer_runner runner(loggers, vm);
		runner.run();
	}

	return 0;
}
