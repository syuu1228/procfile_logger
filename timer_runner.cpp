#include "timer_runner.hpp"

using namespace std;
using namespace boost::program_options;

timer_runner::timer_runner(
	vector<shared_ptr<procfile_logger> >& _loggers, variables_map& vm) :
	loggers(_loggers),
	duration(vm["duration"].as<int>()), 
	terminate(vm["terminate"].as<int>())
{
};

void timer_runner::run()
{
	for (int i = 0;
		terminate != 0 ? i <= terminate : true; i++) {
		for (auto l : loggers) {
			l->update();
			if (i == 0)
				l->start();
		}
		sleep(duration);
	}
}

