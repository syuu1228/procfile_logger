#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"
#include "interrupts.hpp"
#include "netstat_tcpext.hpp"
#include "netstat_ipext.hpp"
#include "stat.hpp"

using namespace std;
using namespace boost::program_options;

int main(int argc, char **argv)
{
	options_description opt("options");
	opt.add_options()
		("help,h", "show help")
		("target,t",
			value<vector<string> >()->composing(),
		 	"target procfiles(interrupts,netstat_tcpext,netstat_ipext,stat)")
		("output,o",
			value<string>()->default_value("."), "output dir")
		("duration,d", 
			value<int>()->default_value(1), "duration sec")
		("terminate,T", 
			value<int>()->default_value(60), "terminate sec");
	variables_map vm;
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
		for (int i = 0; i <= vm["terminate"].as<int>(); i++) {
			for (auto l : loggers) {
				l->update();
				if (i == 0)
					l->start();
			}
			sleep(vm["duration"].as<int>());
		}
	}

	return 0;
}
