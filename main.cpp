#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include "timer_runner.hpp"
#if notyet
#include "procwatch_runner.hpp"
#endif
#include "interrupts.hpp"
#include "netstat_tcpext.hpp"
#include "netstat_ipext.hpp"
#include "stat.hpp"

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
		"target procfiles(interrupts,netstat_tcpext,netstat_ipext,stat)")
		("output,o",
		value<string>()->default_value("."), "output dir")
		("runner_type,r",
		value<string>()->default_value("timer"),
		"runner type(timer,netserver,iperf)")
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
		vector<shared_ptr<procfile_logger> > loggers;
		for (auto t : targets) {
			if (t == "interrupts") {
				loggers.push_back(init_interrupts_logger(vm));
			}else if(t == "netstat_tcpext") {
				loggers.push_back(init_netstat_tcpext_logger(vm));
			}else if(t == "netstat_ipext") {
				loggers.push_back(init_netstat_ipext_logger(vm));
			}else if(t == "stat") {
				loggers.push_back(init_stat_logger(vm));
			}else{
				cout << opt << endl;
				return 1;
			}
		}
		string runner_type = vm["runner_type"].as<string>();
		if (runner_type == "timer") {
			timer_runner runner(loggers, vm);
			runner.run();
		}else if (runner_type == "netserver") {
#if notyet
			procwatch_runner runner(loggers, vm, "netserver", 1);
			runner.run();
#endif
		}else if (runner_type == "iperf") {
#if notyet
#endif
		}else{
			cout << opt << endl;
			return 1;
		}
	}

	return 0;
}
