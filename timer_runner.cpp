#include "timer_runner.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

timer_runner::timer_runner(
	vector<std::shared_ptr<global_stats_logger> >& _gloggers, 
	vector<std::shared_ptr<local_stats_logger> >& _lloggers, 
	variables_map& vm) :
	gloggers(_gloggers),
	lloggers(_lloggers),
	duration(vm["duration"].as<int>()), 
	terminate(vm["terminate"].as<int>()),
	comm(vm.count("process_name") ? vm["process_name"].as<string>() : "")
{
};

void timer_runner::run() throw(no_logger_registered)
{
	if (lloggers.size() == 0 && gloggers.size() == 0) 
		throw no_logger_registered();

	for (int i = 0;
		terminate != 0 ? i <= terminate : true; i++) {
		if (comm != "") {
			filesystem::path proc("/proc");
			filesystem::directory_iterator end;

			for (filesystem::directory_iterator it(proc);
				it != end; ++it) {
				const string filename = it->path().filename().string();
				bool is_numeric = false;
				try {
					stoi(filename);
					is_numeric = true;
				} catch (invalid_argument) {}
	
				if (filesystem::is_directory(*it) && is_numeric) {
					ifstream comm_file(it->path().string() + "/comm");
					string val;
	
					comm_file >> val;
					if (val != comm)
						continue;
					
					for (auto l : lloggers) 
						l->handle_process(it);
				}
			}

			for (auto l : lloggers)  {
				l->update();
				if (i == 0)
					l->start();
			}
		}

		for (auto l : gloggers) {
			l->update();
			if (i == 0)
				l->start();
		}
		sleep(duration);
	}
}


