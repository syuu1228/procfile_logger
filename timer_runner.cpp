#include "timer_runner.hpp"
#include <signal.h>

using namespace std;
using namespace boost;
using namespace boost::program_options;

static volatile sig_atomic_t signaled = 0;

static void sig_handler(int signum) {
	signaled = 1;
}

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

void timer_runner::run() throw(no_logger_registered, sighandler_register_error)
{
	if (lloggers.size() == 0 && gloggers.size() == 0) 
		throw no_logger_registered();

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		throw sighandler_register_error();

	for (int i = 0;
		terminate != 0 ? i <= terminate && !signaled : !signaled; i++) {
		if (comm != "") {
			filesystem::path procfs("/proc");
			filesystem::directory_iterator end;

			for (filesystem::directory_iterator proc(procfs);
				proc != end; ++proc) {
				const string filename = proc->path().filename().string();
				bool is_numeric = false;
				try {
					stoi(filename);
					is_numeric = true;
				} catch (invalid_argument) {}
	
				if (filesystem::is_directory(*proc) && is_numeric) {
					ifstream comm_file(proc->path().string() + "/comm");
					string val;
	
					comm_file >> val;
					if (val != comm)
						continue;

					filesystem::path task(proc->path().string() + "/task");
					for (filesystem::directory_iterator thread(task);
						thread != end; ++thread) {
						for (auto l : lloggers) 
							l->handle_process(thread);
					}
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


