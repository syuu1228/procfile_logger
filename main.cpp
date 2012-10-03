#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include "timer_runner.hpp"
#include "interrupts.hpp"
#include "netstat.hpp"
#include "stat.hpp"
#include "schedstat.hpp"
#include "proc_sched.hpp"
#include "proc_nprocs.hpp"
#include "sha_thread.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

int main(int argc, char **argv)
{
	options_description opt("options");
	variables_map vm;
	vector<std::shared_ptr<global_stats_logger> > gloggers;
	vector<std::shared_ptr<local_stats_logger> > lloggers;
	vector<std::shared_ptr<thread> > threads;

	opt.add_options()
		("help,h", "show help")
		("global_target,g",
		value<vector<string> >()->composing(),
		"target procfiles(interrupts,netstat,stat,schedstat)")
		("process_name,p",
		value<string>(),
		"process name filter for local_target(required for local_target)")
		("local_target,l",
		value<vector<string> >()->composing(),
		"target procfiles(sched,nprocs)")
		("sha_thread,s", "sha thread")
		("output,o",
		value<string>()->default_value("."), "output dir")
		("duration,d", 
		value<int>()->default_value(1), "duration sec")
		("terminate,T", 
		value<int>()->default_value(0), "terminate sec");
	store(parse_command_line(argc, argv, opt), vm);
	notify(vm);

	if (vm.count("help") || 
		(!vm.count("global_target") && !vm.count("local_target"))) {
		cout << opt << endl;
		return 1;
	}

	if (vm.count("global_target")) {
		vector<string> gtargets = vm["global_target"].as<vector<string> >();
		for (auto t : gtargets) {
			if (t == "interrupts") {
				init_interrupts_logger(vm, gloggers);
			}else if(t == "netstat") {
				init_netstat_logger(vm, gloggers);
			}else if(t == "stat") {
				init_stat_logger(vm, gloggers);
			}else if(t == "schedstat") {
				init_schedstat_logger(vm, gloggers);
			}else{
				cout << opt << endl;
				return 1;
			}
		}
	} 

	if (vm.count("local_target")) {
		vector<string> ltargets = vm["local_target"].as<vector<string> >();
		if (vm.count("process_name") == 0) {
			cout << opt << endl;
			return 1;
		}
		for (auto t : ltargets) {
			if(t == "sched") {
				init_proc_sched_logger(vm, lloggers);
			}else if(t == "nprocs") {
				init_proc_nprocs_logger(vm, lloggers);
			}else{
				cout << opt << endl;
				return 1;
			}
		}
	}
	if (vm.count("sha_thread")) {
		init_sha_thread(vm, threads);
	}
	{
		timer_runner runner(gloggers, lloggers, threads, vm);
		runner.run();
	}

	return 0;
}
